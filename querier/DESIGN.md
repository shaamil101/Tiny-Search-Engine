# DESIGN.md - Shaamil Karim Shaw Alem

The TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

### User interface

The querier's only interface with the user is on the command-line;  exactly two command-line arguments.

```bash
$ querier pageDirectory indexFilename
```

- `pageDirectory` is the pathname of a directory produced by the Crawler
- `indexFilename` is the pathname of a file that can be read

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

1. start from a command line 
2. parse the command line, validate parameters, and initialize other modules
3. open the Crawler generated directory and verify it as such
4. ensure the provided index filename is valid and readable
5. read from the index file and load its contents into an inverted-index data structure
6. loop and accept individual queries until ^D is entered
  * 6.1 - check if input is empty or composed of all spaces
  * 6.2 - separate input into individual tokens based on a " " delimiter
  * 6.3 - store tokens into an array of strings
  * 6.4 - check if the token array matches the Backus-Naur Form structure
  * 6.5 - split the array of tokens into andSequences and orSequences
    * 6.5.1 - determine the intersection of all words in each andSequence
    * 6.5.2 - determine the union of all words in orSequences
    * 6.5.3 - produce an array holding all matched documents for the query
  * 6.6 - sort the array of matched documents in decreasing order
  * 6.7 - print query results to the user
7. exit if prompted

### Major data structures

 1. ***index*** of word -> (docID, count) pair mappings
 2. ***counterset*** of (docID, count) pairs

 1. ***counterPair*** that contain two counters at a time
 2. ***document*** that contains a document's docID and score for a matched query
 3. ***ranking*** that contains an array of documents and its length
