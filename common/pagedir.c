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

/**************** global functions ****************/
/* that is, visible outside this fi le */
/* see pagedir.h for comments about exported functions */
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_init() ****************/
/* see pagedir.h for description */
bool pagedir_init(const char *pageDirectory) {
    
  char *saveFile = ".crawler";
    char *filename = mem_calloc_assert(strlen(pageDirectory) + strlen(saveFile) + 2, sizeof(char), "Filename creation failed");

    if (filename == NULL) { // Make sure alloc worked properly
        fprintf(stderr, "Error: Failed to allocate filename for ./crawler\n");
        return false;
    }

    // Populate filename
    strcpy(filename, pageDirectory);
    strcpy(filename + strlen(pageDirectory), "/");
    strcpy(filename + strlen(pageDirectory) + 1, saveFile);

    // Null terminate filename string
    filename[strlen(pageDirectory) + strlen(saveFile) + 1] = '\0';

    FILE *outputFile = mem_assert(fopen(filename, "w"),"Directory is invalid");
    
    // Free memory / close file
    free(filename);
    fclose(outputFile);
return(true);
}

/**************** pagedir_save() ****************/
/* see pagedir.h for description */
void pagedir_save(const webpage_t *page, const char *pageDirectory, const int docID) {
    // Convert ID to a string
    char strID[12];
    sprintf(strID, "%d", docID); // Mallocs space!!
    assert(strID != NULL);

    // Allocate memory for filename and write in form of 'pageDirectory/strID'
    char *filename = calloc(strlen(pageDirectory) + strlen(strID) + 2,sizeof(char));
    assert(filename != NULL);

    //Creating the full pathname
    strcpy(filename, pageDirectory);
    strcpy(filename + strlen(pageDirectory), "/");
    strcpy(filename + strlen(pageDirectory) + 1, strID);

    // Null terminate filename string
    filename[strlen(pageDirectory) + strlen(strID) + 1] = '\0';

    // Open file for writing
    FILE *outputFile = fopen(filename, "w");
    if(outputFile == NULL)
    {
        printf("outputFIle is NULL");
        return;
    }
    if (fprintf(outputFile, "%s\n", webpage_getURL(page)) < 0) {
      printf("Could not write URL out to webpage\n");
      return;                         // false if getting URL failed
    }

    // Write out depth to file
    if (fprintf(outputFile, "%d\n", webpage_getDepth(page)) < 0) {
      printf("Could not write depth out to webpage\n");
      return;                         // false if getting depth failed
    }

    // Write out HTML to file
    if (fprintf(outputFile, "%s", webpage_getHTML(page)) < 0) {
      printf("Could not write HTML out to webpage\n");
      return;                         // false if getting HTML failed
    }
    //freeing filename and closing the output file
    free(filename);
    fclose(outputFile);
}

bool pagedir_validate(const char* pageDirectory)
{
  DIR *dir = opendir(pageDirectory);  // pointer to a directory object

  // if the directory exists, initialize an .crawler file within it
  if (dir) {
    char crawlerAuthFile[100];                // allocate space for filename
    strcpy(crawlerAuthFile, pageDirectory);   // initialize to directory pathway
    strcat(crawlerAuthFile, "/.crawler");      // append /.exists

    FILE *fp = fopen(crawlerAuthFile, "w");   // file pointer to {pageDirectory}/.exists
    if (fp == NULL) {
      printf("File %s does not exist!\n", crawlerAuthFile);
      return false;                   // false if fopen() failed
    }

    fclose(fp);                       // close file
  }
  // return false if directory does not exist
  else {
    printf("Directory does not exist or is otherwise unopenable!\n");
    return false;
  }

  closedir(dir);                      // close directory
  return true;
}

/**************** () ****************/
/* see pagedir.h for description */
bool pagedir_load(const char *pageDir, const webpage_t *wp)
{
  FILE *webpagefp = fopen(pageDir, "w");    // new file pointer

  // if file opened, write out URL, depth, and HTML within it
  if (webpagefp != NULL) {

    // Write out URL to file
    if (fprintf(webpagefp, "%s\n", webpage_getURL(wp)) < 0) {
      printf("Could not write URL out to webpage\n");
      return false;                         // false if getting URL failed
    }

    // Write out depth to file
    if (fprintf(webpagefp, "%d\n", webpage_getDepth(wp)) < 0) {
      printf("Could not write depth out to webpage\n");
      return false;                         // false if getting depth failed
    }

    // Write out HTML to file
    if (fprintf(webpagefp, "%s\n", webpage_getHTML(wp)) < 0) {
      printf("Could not write HTML out to webpage\n");
      return false;                         // false if getting HTML failed
    }

    fclose(webpagefp);                      // close file pointer
  }
  return true;                              // success!
}

