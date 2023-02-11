/* 
 * pagedir.h - header file for CS50 'pagedir' module
 * 
 * pagedir is a a re-usable module pagedir.c to handles the pagesaver mentioned in the design 
 * (writing a page to the pageDirectory), and marking it as a Crawler-produced pageDirectory (as required in the spec). 
 * We chose to write this as a separate module, in ../common, to encapsulate all the knowledge about how to initialize 
 * and validate a pageDirectory, and how to write and read page files, in one place... anticipating future use by 
 * the Indexer and Querier.
 *
 * Shaamil Karim Shaw Alem, 2023
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
/**************** pagedir_validate ****************/
/* Validate whether the directory can be opened
 *
 * Caller provides:
 *   a char* pagedirectory
 * We return:
 *  false if the directory was not valid;
 *  true iff valid.
 * 
 */
bool pagedir_validate(const char* pageDirectory);
/**************** pagedir_init ****************/
/* construct the pathname for the .crawler file in that directory
*open the file for writing; on error, return false.
*close the file and return true.
 *
 * Caller provides:
 *   a char* pagedirectory
 * We return:
 *  false if the directory was not writeable;
 *  true iff written.
 * 
 */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* construct the pathname for the page file in pageDirectory. open that file for writing.print the URL then the
 * the depth, then  the contents of the webpage. Finally, close the file.
 *
 * Caller provides:
 *   a webpage_t* page, char* pagedirectory, int docID
 * We return:
 * Nothing
 * 
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

#endif