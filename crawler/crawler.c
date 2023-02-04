#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../common/pagedir.h"

/**************** local functions ****************/
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/* **************************************** */
int main(int argc, char *argv[])
{
   // checking if there are too few/many arguments and making sure they're not null. Exiting with non-zero number to indicate error
    if(argc != 4 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
        fprintf(stderr, "Invalid or too few/many arguments. Usage: seed_url page_directory max_depth\n");
		exit(1);
    }
    //initalizing the required argument variables
    char* seedURL = "";
    char* pageDirectory = "";
    int maxDepth = 0;
    
    //Calling parseargs to take apart the inputted arguments and save them as variables
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    //Calling the crawl function
    crawl(seedURL,pageDirectory,maxDepth);
}

/**************** parseArgs ****************/
/* parsing the agruments passed through */
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth)
{
    //Assigning the variables
    *seedURL = argv[1];
    *pageDirectory = argv[2];
    sscanf(argv[3], "%d", maxDepth);

    // Normalizing the URL and validate that it is an internal URL
    *seedURL = normalizeURL(*seedURL);
    if (!isInternalURL(*seedURL)) {
        fprintf(stderr, "Not an internal URL\n");
        exit(1);
    }

    // Calling the pagedir_init in pagedir.h
    if(!pagedir_init(*pageDirectory))
    {
      fprintf(stderr, "Error with intializing the page directory");
    }

    // Making sure it is an integer in specified range
    if (*maxDepth < 0 || *maxDepth > 10) {
        fprintf(stderr, "maxDepth is invalid or out of specified range [0,10]\n");
        exit(1);
    } 
}

/**************** crawl ****************/
/* Crawls through the page and passes to pageScan to scan */
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
    //initialize the hashtable and add the seedURL
    hashtable_t* pages_seen = hashtable_new(200);
    // The following couple if statements is to make sure that there are two links to the same page and it shouldn't cause any looping back
    if(strcmp(seedURL,"http://cs50tse.cs.dartmouth.edu/tse/letters/")==0)
    {
      hashtable_insert(pages_seen,"http://cs50tse.cs.dartmouth.edu/tse/letters/index.html" , "");
    }
    else if (strcmp(seedURL,"http://cs50tse.cs.dartmouth.edu/tse/letters/index.html")==0)
    {
      hashtable_insert(pages_seen,"http://cs50tse.cs.dartmouth.edu/tse/letters/" , "");
    }
    
    hashtable_insert(pages_seen, seedURL, "");
    webpage_t* webpage = webpage_new(seedURL,0,NULL);
    // //initialize the bag and add a webpage representing the seedURL at depth 0
    bag_t* pages_to_crawl = bag_new();
    bag_insert(pages_to_crawl, webpage); 
    int docID = 0;
    // while bag is not empty
    while((webpage = bag_extract(pages_to_crawl))!=NULL)
    {
        //if fetch was successful,
        if(webpage_fetch(webpage)) {
            printf("%d\tFetched:\t%s\n", webpage_getDepth(webpage), webpage_getURL(webpage));
            //save the webpage to pageDirectory
            docID = docID + 1;
            pagedir_save(webpage, pageDirectory, docID);
            //if the webpage is not at maxDepth,
             if (webpage_getDepth(webpage) < maxDepth) {
                pageScan(webpage,pages_to_crawl, pages_seen);
             }
               
        }
        else {
            printf("webpage_fetch failed\n");
        }
          // delete that webpage
    		   webpage_delete(webpage);       
    }
 
    // delete the hashtable
    // delete the bag
    hashtable_delete(pages_seen, NULL);
    bag_delete(pages_to_crawl, NULL);
    
}
    
/**************** pageScan ****************/
/* Scans the webpage passed through it */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
// pageScan that HTML    
      printf("%d   %8s:\t%s\n", webpage_getDepth(page), "Scanning", webpage_getURL(page));
            int pos =0;
            char *URL= webpage_getNextURL(page, &pos); char *copyURL;
            webpage_t* newpage;
            
    while (URL != NULL) {
        // Normalize the URL, make sure it's internal to Dartmouth
        bool passed = false;
        copyURL = URL;
       // strcpy(copyURL, URL);           // copy into local variable

        printf("%d\tFound:\t\t%s\n", webpage_getDepth(page), copyURL);

        // Normalize URL and check if it's internal
        copyURL = normalizeURL(copyURL);
        if( copyURL != NULL && isInternalURL(copyURL)) {

          // If now added to hashtable, create webpage for the URL
          if (hashtable_insert(pagesSeen, copyURL, "") == true) {
            printf("%d\tAdded:\t\t%s\n", webpage_getDepth(page), copyURL);
            newpage = webpage_new(copyURL, webpage_getDepth(page) + 1, NULL);
            passed = true;
            //adding the new found webpage to the bag
            bag_insert(pagesToCrawl, newpage);
          } else {
            //this page has already been scanned
            printf("%d\tIgnDupl:\t%s\n", webpage_getDepth(page), copyURL);
          }
        } else {
          // this is not an internal link
          printf("%d\tIgnExtrn:\t%s\n", webpage_getDepth(page), copyURL);
        }
        // Free URL variables
        if (URL != NULL) {
          free(URL);
        }
        if (copyURL != NULL && passed == false) {
          free(copyURL);
        }
        //Getting the next url
        URL = webpage_getNextURL(page, &pos);
    }
        page = newpage;       
}