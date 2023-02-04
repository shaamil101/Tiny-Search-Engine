# Readme Shaamil Karim Shaw Alem

### Crawler
A crawler is one of the parts of the tiny search engine that serves the purpose of parsing through URLs passed through it and collecting all the URLs upto a certain depth and saving the HTML file of each URL for the indexer and querier to use.

### Usage

These are the primary functions available to use by the main function

```c
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
```

### Implementation

`parseArgs`: Given arguments from the command line, extract them into the function parameters; return only if successful.

`crawl`: Do the real work of crawling from seedURL to maxDepth and saving pages in pageDirectory.

`pageScan`: This function implements the pagescanner mentioned in the design. Given a webpage, scan the given page to extract any links (URLs), ignoring non-internal URLs; for any URL not already seen before (i.e., not in the hashtable), add the URL to both the hashtable pages_seen and to the bag pages_to_crawl.


### Files

* `crawler.c` -  implementation
* `testing.sh` - testing file
* `testing.out` - output file from testing.sh
* `Makefile` - compilation

### Assumptions

None

### Compilation

To compile, simply `make` and then `make test`. Upon running toscrape for depth 2, the testing.out file was 10,000+ lines so i removed the test for readability
