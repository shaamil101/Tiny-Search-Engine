/*
 * indexer.c - implements the TSE indexer
 *
 * The job of the indexer is to read the documents in the pageDirectory output
 * by the crawler, build an inverted index mapping from words to documents,
 * and write that index to a file.
 *
 * Shaamil Karim Shaw Alem
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <ctype.h>
#include "../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/file.h"
#include "../common/pagedir.h"
#include "../libcs50/counters.h"
#include "../common/index.h"
#include "../common/word.h"

/**************** functions ****************/
void index(const char* pageDirectory, index_t *wordMap);
void ctrsDelete(void *item);

/**************** main ****************/
/* Validates arguments, initializes modules,
 * and calls index function
 *
 * Caller provides:
 *   two arguments:
 *      1. pageDirectory, the pathway to an existing Crawler-generated directory
 *      3. indexFilename, a file path to which output is directed
 * Assumptions:
 *      1. indexFilename is not longer than 500 characters
 *      2. pageDirectory is not longer than 500 characters
 * We return:
 *   0 if successful execution
 *   else non-zero if any failures
 */
int main(const int argc, const char *argv[])
{
  // Ensure two non-NULL command ilne arguments
  if (argc == 3 && argv[1] != NULL && argv[2] != NULL) {
    char pageDirectory[500];          // allocate pageDirectory copy variable
    strcpy(pageDirectory, argv[1]);   // copy command-line arg into pageDirectory

    // If given directory pathway does not exist, exit with non-zero status
    if (pagedir_validate(pageDirectory) == false) {
      fprintf(stderr,"Inavlid directory %s\n", pageDirectory);
      exit(1);
    }

    char indexFilename[500];          // allocate indexFilename copy variable
    strcpy(indexFilename, argv[2]);   // copy command-line arg into indexFilename

    // Ensure index file is openable/writable; if not, exit with non-zero status
    FILE *fpIndexFilename = fopen(indexFilename, "w");
    if (fpIndexFilename == NULL) {
     fprintf(stderr,"Not possible to open %s\n", indexFilename);
      exit(2);
    }

    index_t *wordMap = index_new(800);    // initialize inverted-index structure
    index(pageDirectory, wordMap);                // analyze word frequencies

    indexFileGenerate(wordMap, indexFilename);    // write output to indexFilename

    // Clean up memory allocations
    index_delete(wordMap);
    fclose(fpIndexFilename);
  }
  // If invalid command line arguments, exit with a non-zero status
  else {
    fprintf(stderr, "Exactly two non-NULL command-line arguments need to be entered\n");
    exit(3);
  }

  return 0;
}

/**************** index ****************/
/* Called by main(): loops through all documents in
 * the crawler-generated directory and adds relevant
 * information to the inverted-index index structure
 *
 * Input: pageDirectory, and wordMap
 * Output: an inverted-index index
 *
 * Assumptions:
 *   1. The file pathway (e.g. ../data/1) is not longer
 *      than 500 characters
 * We return:
 *   0 if all words in all documents added to index
 *   else non-zero if any failures
 */
void index(const char* pageDirectory, index_t *wordMap)
{
  int intDocID = 1;                        // a "counter" of document IDs

  while (true) {
    // Creates a document path (e.g. "../data/1")
    char strDocID[12];                     // document ID
    sprintf(strDocID, "/%d", intDocID);
    char docDir[500];                      // document directory
    strcpy(docDir, pageDirectory);
    strcat(docDir, strDocID);

    FILE *docfp = fopen(docDir, "r");      // file pointer to current document

    // Exit if any file errors
    if (docfp == NULL) {
      break;
    }

    char *url = file_readLine(docfp);         // reads in the URL from the document
    char *strDepth = file_readLine(docfp);    // reads in the depth
    int depth = atoi(strDepth);
    char *html = file_readFile(docfp);        // reads in the HTML content

    // Initialize a webpage for the current document
    webpage_t *page = webpage_new(url, depth, html);

    // If any webpage errors, exit immediately
    if (page == NULL) {
      fprintf(stderr,"Error creating a webpage from the given URL, depth, and HTML content\n");
      exit(4);
    }

    int pos = 0;      // tracks position in HTML
    char *word;       // tracks current word in HTMl
    char wordCopy[100]; // copy of word

    // For each word, add it to the inverted-index index
    while ((word = webpage_getNextWord(page, &pos)) != NULL) {
      strcpy(wordCopy, word);
      // Check if word is less than 3 characters
      if (strlen(wordCopy) >= 3.0) {
        normalizeWord(wordCopy);
        // Initialize a counterset for this particular word
        counters_t *counterset = index_find(wordMap, wordCopy);
        if (counterset != NULL) {
          counters_add(counterset, intDocID);
        } else {
          counterset = counters_new();
          if (counterset == NULL) {
            fprintf(stderr, "Error creating a bag or counterset for the word %s\n", wordCopy);
            exit(5);
          }
          counters_add(counterset, intDocID);             // add (docID, count)
          index_insert(wordMap, wordCopy, counterset);    // insert into the inverted-index structure
        }
      }

      free(word);     // free word instance
    }

    intDocID++;       // increment to the next document ID

    // Clean up
    fclose(docfp);
    free(url);
    free(strDepth);
    free(html);
    free(page);
  }
}

/**************** ctrsDelete ****************/
/* Helper function for hashtable_delete to delete
 * counterset items */
void ctrsDelete(void *item)
{
  counters_t *ctrs = item;
  counters_delete(ctrs);
}
