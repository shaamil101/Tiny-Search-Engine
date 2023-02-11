/*
 * indextest.c - validates the output from indexer
 *
 * The indexer writes the inverted index to a file, and both the index tester and the querier read the inverted index from a file
 *
 * Shaamil Shaw Alem
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <ctype.h>
#include "../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../libcs50/set.h"
#include "../libcs50/file.h"
#include "../common/pagedir.h"
#include "../libcs50/counters.h"
#include "../common/index.h"
#include "../common/word.h"

/**************** local functions ****************/
void ctrsDelete(void *item);

/**************** main ****************/
/* Reads input file, builds inverted-index
 * index, and produces similar output file
 *
 * Caller provides:
 *   execute from a command line with usage syntax
        ./indextest oldIndexFilename newIndexFilename
          where oldIndexFilename is the name of a file produced by the indexer, and
          where newIndexFilename is the name of a file into which the index should be written.
 * Assumptions:
 *      1. oldIndexFilename max size is 500 characters
 *      2. newIndexFilename max size 500 characters
 *
 */
int main(const int argc, const char *argv[])
{
  // Ensure there are only two non-NULL command line arguments
  if (argc == 3 && argv[1] != NULL && argv[2] != NULL) {
    char oldIndexFilename[500];
    strcpy(oldIndexFilename, argv[1]);

    // Creating a copy of the new indexFilename
    char newIndexFilename[500];
    strcpy(newIndexFilename, argv[2]);
    // initialize inverted-index module from index.h
    index_t *index = index_new(800);      

    //open the files
    FILE *oldFile = fopen(oldIndexFilename, "r");
    FILE *newFile = fopen(newIndexFilename, "w");


    int documentID = -1;
    int count = -1;                     
    int numLines = file_numLines(oldFile);            

    // For each line, add its word->(documentID, count) mapping to the index
    for (int i = 0; i < numLines; i++) {
      //storing the weard read from the old file
      char *word = file_readWord(oldFile);        
      counters_t *ctrs = counters_new();      
      //insert into index data structure from index.h
      index_insert(index, word, ctrs);  

      // If any memory allocation errors, exit 
      if (ctrs == NULL) {
        printf("Was not able to initialize a counterset for the word %s\n", word);
        exit(1);
      }

      // Add each pair to the word's counterset
      while(fscanf(oldFile, "%d %d ", &documentID, &count) == 2) {
        counters_set(ctrs, documentID, count);
      }
      //freeing the memory in word
      free(word);      
    }

    // Generates output to the new index file using the newly built index
    indexFileGenerate(index, newIndexFilename);

    // Freeing up the memory
    index_delete(index);
    fclose(oldFile);
    fclose(newFile);
  }

  return 0;
}

/**************** ctrsDelete ***************/
/* Helper function to delete the counters items from the index*/
void ctrsDelete(void *item)
{
  counters_t *ctrs = item;
  counters_delete(ctrs);
}
