
### Implementation - Shaamil Kairm Shaw Alem

There are two main functions that form the querier module: `main` and `query`.

Pseudo code for `int main(const int argc, const char *argv[])`:
1. Validate the number of command line arguments by ensuring `argc` equals 3 and that none of the arguments are null
2. Check if `pageDirectory` is a valid Crawler-generated directory by checking for an `.crawler` file
3. Check if `indexFilename` is the path to a valid index file name 
4. Initialize a `index` and call the `query(...)` function on it
5. Clean up and exit with a zero status

Pseudo code for `void query(char *pageDirectory, index_t *wordMap)`:

1. - Loop infinitely, prompting for a query each time until *^D* is encountered
2. - Check for EOF with `feof(stdin)` and if the input is empty
3. - Ensure query validity
4. - Allocate an array of character pointers to eventually store query tokens within
5. - Use `strtok(...)` to split the original query input into individual tokens  
  * 5.1 - Use `normalizeWord(token)` to normalize each token before adding it to the array
6. - Call `isBNF(...)` to ensure the array of tokens follows the Backus-Naur Form structure
7. - Call `query(...)` on the array of tokens to receive a `counterset` storing all (docID, score) pairs that  the overall query
  * 7.1 - Note that other functions are called to assist `query(...)`, including:
    * `queryHelper(...)`
    * `andSequence(...)`
    * `andSequenceHelper(...)`
8. - Create an array of *documents*
9. - Iterate through the `counterset` of matched documents:
  * 9.1 - Call `rank(...)` to insert each document into the array of documents 
10. - Print out information about each matched document

### Major data structures

 1. ***index*** of word -> (docID, count) pair mappings
 2. ***counterset*** of (docID, count) pairs

 1. ***counterPair*** that contain two counters at a time
 2. ***document*** that contains a document's docID and score for a matched query
 3. ***ranking*** that contains an array of documents and its length
