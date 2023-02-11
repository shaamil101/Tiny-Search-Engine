/* 
 * index.h - header file for CS50 'index' module
 * 
 * Index is a a re-usable module index.c to handles the index mentioned in the design 
 * 
 * a module to implement an abstract index_t type that represents an index in memory,
 *  and supports functions like index_new(), index_delete(), index_save(), and so forth. 
 * Tip: much of it is a wrapper for a hashtable.
 *
 * Shaamil Karim Shaw Alem, 2023
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include "../common/pagedir.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"

/**************** global types ****************/
typedef hashtable_t index_t; // opaque to users of the module

/**************** functions ****************/

/**************** index_new ****************/
/* Create a new (empty) hashtable.
 *
 * Caller provides:
 *   number of slots to be used for the hashtable (must be > 0).
 * We return:
 *   pointer to the new hashtable; return NULL if error.
 * We guarantee:
 *   hashtable is initialized empty.
 * Caller is responsible for:
 *   later calling hashtable_delete.
 */
index_t *index_new(const int num_slots);

/**************** index_insert ****************/
/* Insert item, identified by key (string), into the given hashtable.
 *
 * Caller provides:
 *   valid pointer to index, valid string for key, valid pointer for item.
 * We return:
 *   false if key exists in ht, any parameter is NULL, or error;
 *   true iff new item was inserted.
 * Notes:
 *   The key string is copied for use by the index; that is, the module
 *   is responsible for allocating memory for a copy of the key string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its key string after this call.  
 */
bool index_insert(index_t *ht, char *key, counters_t *ctrs);

/**************** index_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid pointer to index, valid string for key.
 * We return:
 *   pointer to the item corresponding to the given key, if found;
 *   NULL if index is NULL, key is NULL, or key is not found.
 * Notes:
 *   the index is unchanged by this operation.
 */
counters_t *index_find(index_t *ht, const char *key);

/**************** index_delete ****************/
/* Delete index, calling a delete function on each item.
 *
 * Caller provides:
 *   valid index pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if index==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, call the itemfunc on each item.
 *   free all the key strings, and the index itself.
 * Notes:
 *   We free the strings that represent key for each item, because 
 *   this module allocated that memory in index_insert.
 */
void index_delete(index_t *ht);

/**************** index_iterate ****************/
/* Iterate over all items in the table; in undefined order.
 *
 * Caller provides:
 *   valid pointer to index, 
 *   arbitrary void*arg pointer,
 *   itemfunc that can handle a single (key, item) pair.
 * We do:
 *   nothing, if ht==NULL or itemfunc==NULL.
 *   otherwise, call the itemfunc once for each item, with (arg, key, item).
 * Notes:
 *   the order in which index items are handled is undefined.
 *   the index and its contents are not changed by this function,
 *   but the itemfunc may change the contents of the item.
 */
void index_iterate(index_t *ht, void *arg, void(*itemfunc)(void *arg, const char *key, void *item));

/**************** indexFileGenerate ****************/
/* Iterates through the index to write output
 * out to the index file
 *
 * Caller provides:
 *   inverted-index index structure
 *   string representing the index file pathway
 * We return:
 *   true if index file generated
 *   otherwise false if any failures
 */
bool indexFileGenerate(index_t *wordMap, const char *indexFilename);

/**************** indexFileHelper ****************/
/* Helper function that writes out the word to
 * the index file and iterates through the counterset
 *
 * Caller provides:
 *   file poiner to index file (arg)
 *   index key
 *   index item
 */
void indexFileHelper(void *arg, const char *key, void *item);

/**************** counterPrint ****************/
/* Helper function that writes out the (docID, count)
 * pair to the index file
 *
 * Caller provides:
 *   file poiner to index file (arg)
 *   counterset key
 *   counterset item
 */
void counterPrint(void *arg, const int key, const int count);

#endif
