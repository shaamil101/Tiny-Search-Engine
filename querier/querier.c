/*
 * querier.c - TSE querier
 *
 * The TSE Querier is a standalone program that makes use of the output from TSE Crawler
 * and the TSE Querier and answers search queries
 * queries submitted via stdin.
 *
 * Shaamil Shaw Alem
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include "../common/index.h"
#include "set.h"
#include "counters.h"
#include "file.h"
#include "webpage.h"
#include "pagedir.h"
#include "word.h"

/**************** local types ****************/
typedef struct counterPair {
  counters_t *counter1;  // first set
  counters_t *counter2;  // second set
} counterPair_t;

typedef struct document {
  int docID;  // document ID
  int score;  // document's score
} document_t;

typedef struct rankingData {
  struct document *ranking; // array of documents
  int length;               // array's length
} rankingData_t;

/**************** functions ****************/
void findNumDocsMatched(void *arg, const int key, const int count);
void rank(void *arg, const int key, const int count);
void printDocuments(char *pageDirectory, struct document *ranking, int numDocsMatched);
bool isValid(char *input);
int fileno(FILE *stream);
static void prompt(void);
void query(char *pageDirectory, index_t *index);
void ctrsDelete(void *item);
bool loadIndex(const int numberOfLiness, FILE *fpIndexFilename, index_t *index, int docID, int count);
bool isBlank(char *input);
bool isBNF(char *words[]);
counters_t *andSequence(char *words[], int startingIndex, index_t *index);
void andSequenceHelper(void *arg, const int key, const int count);
counters_t *queryMatch(char *words[], index_t *index);
void queryHelper(void *arg, const int key, const int count);

/**************** main ****************/
/* Validates arguments, creates modules,
 * and calls query function
 *
 * Caller provides:
 *   two arguments:
 *      1. pageDirectory, the pathway to an existing Crawler-generated directory
 *      3. indexFilename, a file path through which input is read
 * Assumptions:
 *      1. indexFilename is not longer than 500 characters
 *      2. pageDirectory is not longer than 500 characters
 * We exit:
 *   0 if successful execution
 *   else non-zero if any failures
 */
int main(const int argc, const char *argv[])
{
  // Validate number of command-line arguments
  if (argc == 3 && argv[1] != NULL && argv[2] != NULL) {
    char pageDirectory[500];          // allocate pageDirectory copy variable
    strcpy(pageDirectory, argv[1]);   // copy command-line arg into pageDirectory
    pagedir_validate(pageDirectory);

    char indexFilename[500];          // allocate indexFilename copy variable
    strcpy(indexFilename, argv[2]);   // copy command-line arg into indexFilename

    // Ensure index file is readable; if not, exit with non-zero status
    FILE *fpIndexFilename = fopen(indexFilename, "r");
    if (fpIndexFilename == NULL) {
      printf("Not possible to open %s for reading\n", indexFilename);
      exit(4);
    }

    int docID = -1, count = -1;                     // a word's associated docID & count measures
    int numberOfLiness = file_numLines(fpIndexFilename);            // the number of lines in the old index file

    index_t *index;        // inverted-index index
    index = index_new(numberOfLiness);
    // if (numLines < 800) {
    //   index = index_new(numLines);      // initialize inverted-index index
    // } else {
    //   index = index_new(800);      // initialize inverted-index index
    // }

    // If index file wasn't loaded properly, exit immediately
    if (!loadIndex(numberOfLiness, fpIndexFilename, index, docID, count)) {
      printf("Was not able to load the index file into an internal data structure\n");
      exit(5);
    }

    // Call query function on the index
    query(pageDirectory, index);

    // Clean up
    index_delete(index);
    fclose(fpIndexFilename);
  } else {
    printf("Exactly two non-NULL command-line arguments need to be entered\n");
    exit(6);
  }

}

/**************** query ****************/
/* Initializes a while loop to collect queries and prints output
 * consisting of documents matching each query
 *
 * Caller provides:
 *   two arguments:
 *      1. pageDirectory, the pathway to an existing Crawler-generated directory
 *      3. index, the inverted-index index holding word->(docID, count) data
 */
void query(char *pageDirectory, index_t *index)
{
  // Loop to keep asking for query input
  while (true) {
    prompt();
    char *input = file_readLine(stdin);    // input

    if (feof(stdin)) {
      free(input);
      break;
    } else if (*input == '\0') {
      printf("Error: input is empty.\n");
    } else {
      // Check for invalid input
      if (isBlank(input)) {
        printf("Error: input is all spaces.\n");
      } else if (!isValid(input)) {
        printf("Error: invalid character in query. Only letters and spaces allowed.\n");
      } else {
        // If input is valid, copy it into a copy variable
        char inputCopy[strlen(input)];
        strcpy(inputCopy, input);           // copy into variable

        const char delim[2] = " ";        // delimiter
        char *token;                      // token variable for separated words
        char *words[strlen(input)];       // initialize array of tokens

        // Initialize all spots with a "_"
        for (int i = 0; i < strlen(input); i++) {
          words[i] = "_";
        }

        int index_num = 0;
        token = strtok(inputCopy, delim);

        // Store each input token into the array of tokens
        while (token != NULL) {
          normalizeWord(token);       // normalize the word
          words[index_num] = token;
          token = strtok(NULL, delim);  // get the next token
          index_num++;
        }

        // Exit if input doesn't follow BNF structure
        if (!isBNF(words)) {
          free(input);
          free(token);
          continue;
        }

        // Print the normalized query for user
        printf("Query: ");
        int i = 0;

        while (strcmp(words[i], "_") != 0) {
          printf("%s ", words[i]);
          i++;
        }
        printf("\n");

        // Counterset representing the matched documents for the given query
        counters_t *query = queryMatch(words, index);

        // Determine the number of matched documents
        int numDocsMatched = 0;
        counters_iterate(query, &numDocsMatched, findNumDocsMatched);

        // Allocate array of documents
        struct document ranking[numDocsMatched];
        for (int i = 0; i < numDocsMatched; i++) {
          struct document null = {-1, -1};
          ranking[i] = null;
        }

        // Add each matched document to the array of documents and sort in decreasing order
        rankingData_t rankingData = {ranking, numDocsMatched};
        counters_iterate(query, &rankingData, rank);

        // Print output for user
        printDocuments(pageDirectory, ranking, numDocsMatched);

        if (query != NULL) {
          counters_delete(query);
        }
        free(token);
      }
    }
    free(input);
  }
}

/**************** printDocuments ****************/
/* Prints out the report of matched documents to stdout */
void printDocuments(char *pageDirectory, struct document *ranking, int numDocsMatched)
{
  printf("Matches %d documents (ranked):\n", numDocsMatched);
  for (int i = 0; i < numDocsMatched; i++) {
    char strDocID[12];                     // document ID
    sprintf(strDocID, "/%d", ranking[i].docID);
    char docDir[500];                      // document directory
    strcpy(docDir, pageDirectory);
    strcat(docDir, strDocID);

    FILE *docfp = fopen(docDir, "r");      // file pointer to current document

    // Exit if any file errors
    if (docfp == NULL) {
      break;
    }

    char *url = file_readLine(docfp);         // reads in the URL from the document

    // Print score, document ID, and URL for each matched document
    printf("score\t%d\tdoc\t%d: %s\n", ranking[i].score, ranking[i].docID, url);

    // Clean up
    free(url);
    fclose(docfp);
  }
}

/**************** ctrsDelete ****************/
/* Helper function for index_delete to delete
 * counterset items */
void ctrsDelete(void *item)
{
  counters_t *ctrs = item;
  counters_delete(ctrs);
}

/**************** loadIndex ****************/
/* Loads the inverted-index index */
bool loadIndex(const int numberOfLiness, FILE *fpIndexFilename, index_t *index, int docID, int count) {
  // For each line, add its word->(docID, count) mapping to the index
  for (int i = 0; i < numberOfLiness; i++) {
    char *word = file_readWord(fpIndexFilename);   // read in the word
    counters_t *ctrs = counters_new();      // initialize the word's counterset

    // If any malloc errors, exit immediately
    if (ctrs == NULL) {
      printf("Was not able to initialize a counterset for the word %s\n", word);
      return false;
    }

    index_insert(index, word, ctrs);  // insert into the inverted-index structure

    // Add each (docID, count) pair to the word's counterset
    while(fscanf(fpIndexFilename, "%d %d ", &docID, &count) == 2) {
      counters_set(ctrs, docID, count);
    }

    free(word);       // free the word variable
  }

  return true;
}

/**************** prompt ****************/
/* Prompts with "Query?" if stdin is a tty */
static void prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

/**************** isBlank ****************/
/* Checks if a query is all spaces */
bool isBlank(char *input)
{
  // For each character, return false if a non-space
  for (int i = 0; i < strlen(input); i++) {
    if (isspace(input[i]) == 0) {
      return false;
    }
  }

  return true;
}

/**************** isValid ****************/
/* Checks if a query only has letters and spaces */
bool isValid(char *input)
{
  // For each character, return false if non-letter or non-space
  for (int i = 0; i < strlen(input); i++) {
    if (isspace(input[i]) == 0 && isalpha(input[i]) == 0) {
      return false;
    }
  }

  return true;
}

/**************** isBNF ****************/
/* Checks if a query matches the BNF structure */
bool isBNF(char *words[])
{
  int i = 0;

  // Return false if beginning of query is an operator
  if (strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0) {
    printf("Error: beginning of query cannot be an operator.\n");
    return false;
  }

  // Return false if two operators are adjacent
  while (strcmp(words[i], "_") != 0) {
    // if words[i] == AND/OR and words[i+1] == AND/OR, then adjcaent
    if ((strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0) &&
        (strcmp(words[i+1], "and") == 0 || strcmp(words[i+1], "or") == 0)) {
          printf("Error: two operators adjacent in query.\n");
          return false;
        }
    i++;
  }

  // Return false if end of query is an operator
  i--;
  if (strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0) {
    printf("Error: end of query cannot be an operator.\n");
    return false;
  }

  return true;      // return true if query follows BNF structure
}

/**************** andSequence ****************/
/* Returns a counterset representing the matched documents
   for a given andSequence beginning at the given startingIndex */
counters_t *andSequence(char *words[], int startingIndex, index_t *index)
{
  int i = startingIndex;                  // starting index
  counters_t *result = counters_new();    // initialize "running product" counter

  // For each word in the add-sequence, find its matched documents and add to running product counterset
  if (strcmp(words[i+1], "_") == 0 || strcmp(words[i+1], "or") == 0) {
    counters_t *match = index_find(index, words[i]);
    // If no matches, print error message
    if (match == NULL) {
      printf("No documents matched for %s.\n", words[i]);
    }
    counterPair_t matches = {NULL, result};
    counters_iterate(match, &matches, andSequenceHelper);
    return result;
  }

  i++;

  // Initialize the matches for the first word in an "word1 and word2" pair
  counters_t *match1;

  // Account properly for "and" operators
  if (strcmp(words[i], "and") == 0) {
    i++;
    match1 = index_find(index, words[i-2]);
  } else {
    match1 = index_find(index, words[i-1]);
  }

  // Initialize the matches for the second word in an "word1 and word2" pair
  counters_t *match2 = index_find(index, words[i]);

  // Error if no documents matched
  if (match1 == NULL || match2 == NULL) {
    printf("No documents matched.\n");
    return result;
  }

  // Find the intersection between the matches for both words
  counterPair_t matches = {match2, result};
  counters_iterate(match1, &matches, andSequenceHelper);

  // If end of and-sequence, exit
  if (strcmp(words[i+1], "_") == 0 || strcmp(words[i+1], "or") == 0) {
    return result;
  }

  i++;
  counters_t *extendedResult;

  // Find matches for additional words beyond the first two, if present
  while (strcmp(words[i], "_") != 0 && strcmp(words[i], "or") != 0) {
    if (strcmp(words[i], "and") == 0) {
      i++;      // skip if "and" operator found
    }

    // Find the matched documents for the current word
    extendedResult = counters_new();
    counters_t *match = index_find(index, words[i]);

    // If no matched documents, exit
    if (match == NULL) {
      printf("No documents matched.\n");
      counters_delete(result);
      counters_t *result = counters_new();
      return result;
    }

    // Find the intersection between the "running product" and matches for the current word
    counterPair_t matches = {match, extendedResult};
    counters_iterate(result, &matches, andSequenceHelper);
    result = extendedResult;

    i++;            // increment to the next word in the and-sequence
  }

  return result;    // return final intersection counterset
}

/**************** andSequenceHelper ****************/
/* Helper function for andSequence(). Iterates through
   two countersets (each for separate words) and returns
   the intersection */
void andSequenceHelper(void *arg, const int key, const int count)
{
  // Initialize variables
  counterPair_t *matches = arg;
  counters_t *match2 = matches->counter1;    // matched documents for current word
  counters_t *result = matches->counter2;    // "running product" counterset

  // If single-word and-sequence, copy all documents into "running product"
  if (match2 == NULL) {
    counters_set(result, key, count);
  }
  // If multiple words in the and sequence, find common (docID, count) pairs
  // and add it to the "running product" counterset
  else {
    // If key found, add to counterset
    if (counters_get(match2, key) != 0) {
      // Add the minimum score between the two words
      if (counters_get(match2, key) > count) {
        counters_set(result, key, count);
      } else {
        counters_set(result, key, counters_get(match2, key));
      }
    }
  }
}

/**************** query ****************/
/* Returns a counterset representing the matched documents
   for a given sequence of andSequences (disjunction of conjunctions) */
counters_t *queryMatch(char *words[], index_t *index)
{
  int i = 0;
  counters_t *queryResult = counters_new();     // "running total" counterset

  // Initialize running total counterset with intersection for beginning and-sequence
  counters_t *startIntersection = andSequence(words, 0, index);
  counters_iterate(startIntersection, queryResult, queryHelper);

  // Clean up
  if (startIntersection != NULL) {
    counters_delete(startIntersection);
  }

  // Find intersection for each and-sequence beginning at an "or" operator
  i++;
  while (strcmp(words[i], "_") != 0) {
    // If an "or" operator is found, find the following and-sequence's intersection
    if (strcmp(words[i], "or") == 0) {
      counters_t *intersection = andSequence(words, i+1, index);
      // Add it to the "running total" counterset
      counters_iterate(intersection, queryResult, queryHelper);
      if (intersection != NULL) {
        counters_delete(intersection);
      }
    }
    i++;
  }

  return queryResult;  // return the counterset representing matches for the entire query
}

/**************** queryHelper ****************/
/* Helper function for query(). Iterates through
   a countersets and returns the union  */
void queryHelper(void *arg, const int key, const int count)
{
  counters_t *queryResult = arg;    // initialize counterset
  // Calculate and add the sum of the two scores to the counterset
  counters_set(queryResult, key, count + counters_get(queryResult, key));
}

/**************** findNumDocsMatched ****************/
/* Finds the number of documents that match a query by
   iterating through the counterset that represents query matches */
void findNumDocsMatched(void *arg, const int key, const int count)
{
  int *numDocsMatched = arg;
  (*numDocsMatched)++;
}

/**************** rank ****************/
/* Insertion-sort algorithm that sorts, in
   decreasing order, the documents that match a query
   by each document's score */
void rank(void *arg, const int key, const int count)
{
  // Initialize variables
  rankingData_t *rankingData = arg;
  struct document *ranking = rankingData->ranking;   // array of documents
  int length = rankingData->length;            // length of array of documents
  document_t doc = {key, count};                    // document struct of current key/count

  // Loop through array of documents and add current document to ensure decreasing order
  for (int i = 0; i < length; i++) {
    // Add first document
    if (i == 0 && ranking[0].docID == -1) {
      ranking[0] = doc;
      break;
    }
    // Add second document in order
    else if (i == 1 && ranking[1].docID == -1) {
      if (doc.score > ranking[0].score) {
        ranking[1] = ranking[0];
        ranking[0] = doc;
        break;
      } else {
        ranking[1] = doc;
        break;
      }
    }
    // For all documents after the second, add in decreasing order
    else {
      // If current score is greater than ranking[i]'s score, bump all
      // indexes after by one index and insert document in current spot
      if (doc.score > ranking[i].score) {
        int j = length-1;

        // Move documents forward by one spot
        while (j >= i) {
          ranking[j+1] = ranking[j];
          j--;
        }

        ranking[i] = doc;         // insert current doc
        break;
      }
    }
  }
}
