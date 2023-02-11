#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"
#include <dirent.h>
#include <assert.h>
#include "../common/pagedir.h"
#include "../libcs50/counters.h"
#include "../common/index.h"
#include "../common/word.h"

typedef hashtable_t index_t;
static void delete_helper(void *item);
index_t *index_new(const int num_slots)
{
	index_t *ht;

	ht = hashtable_new(num_slots);
	if (ht == NULL) {
		fprintf(stderr, "failed to allocate index");
		exit(1);
	}
	
	return ht;
}

bool index_insert(index_t *ht, char *key, counters_t *ctrs)
{
	normalizeWord(key);
	return hashtable_insert(ht, key, ctrs);
}
counters_t *index_find(index_t *ht, const char *key)
{
	return (counters_t*) hashtable_find(ht, key);
}

void index_delete(index_t *ht)
{
	hashtable_delete(ht, delete_helper);
}
static void delete_helper(void *item)
{
	if (item) {
		counters_delete((counters_t*) item);
	}
}

/**************** index_iterate() ***************/
void index_iterate(index_t *ht, void *arg, 
			void(*itemfunc)(void *arg, const char *key, void *item))
{
	hashtable_iterate(ht, arg, itemfunc);
}

/**************** indexFileGenerate() ****************/
/* see index.h for description */
bool indexFileGenerate(index_t *wordMap, const char *indexFilename)
{
  FILE *indexfp = fopen(indexFilename, "w");    // new file pointer

  if (indexfp != NULL) {
    // Iterate over each hashtable slot with genIFHelper
    hashtable_iterate(wordMap, indexfp, indexFileHelper);
    fclose(indexfp);  // clean up
  } else {
    return false;
  }

  return true;
}

/**************** indexFileHelper() ****************/
/* see index.h for description */
void indexFileHelper(void *arg, const char *key, void *item)
{
  FILE *indexfp2 = arg;
  counters_t *ctrs = item;

  if (indexfp2 != NULL) {
    // Write out the word to the index file
    if (fprintf(indexfp2, "%s ", key) < 0) {
      fprintf(stderr,"Could not write word %s out to webpage\n", key);
    }
    // Iterate over countersets with counterPrint
    counters_iterate(ctrs, indexfp2, counterPrint);
    fprintf(indexfp2, "\n");
  }
}

/**************** counterPrint() ****************/
/* see index.h for description */
void counterPrint(void *arg, const int key, const int count)
{
  FILE *indexfp3 = arg;

  // Write out the docID and count to the index file
  if (fprintf(indexfp3, "%d %d ", key, count) < 0) {
    fprintf(stderr,"Could not write docID-count pair out to webpage\n");
  }
}
