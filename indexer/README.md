# Readme Shaamil Karim Shaw Alem

```
****************
* EXTENSION USED
****************
```

### Indexer
Indexer is one of the parts of the tiny search engine that serves the purpose of parsing through crawler directory and creating an inverted data structure linking the ekywirds to the documents and webpages it is in.

### Usage

These are the primary functions available to use by the main function

```c
void index(const char* pageDirectory, index_t *wordMap);
void ctrsDelete(void *item);
```

### Implementation

`index`: alled by main(): loops through all documents in the crawler-generated directory and adds relevant information to the inverted-index index structure


### Files

* `indexer.c` -  implementation
* `indextest.c` - testing the index functions
* `testing.sh` - testing file
* `testing.out` - output file from testing.sh
* `Makefile` - compilation

### Assumptions

None

### Compilation

To compile, simply `make` and then `make test`. Upon running toscrape for depth 2, the testing.out file was 10,000+ lines so i removed the test for readability
