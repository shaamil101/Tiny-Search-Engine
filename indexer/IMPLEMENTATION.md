# CS50 Lab 5
## Shaamil Karim

### Implementation

The indexer has three primary parts of code. The main function, the index, and the ctrsDelete helper. We'll go over each one of them and later discuss the testing. We also make use of the index module which will be talked about briefly in the pseudcode.

Pseudo code for `int main(const int argc, const char *argv[])`:
1. Ensure two non-NULL command ilne arguments
2. Check if `pageDirectory` is a valid Crawler-generated directory by checking for an `.crawler` file by calling the pagedir_validate function in the pagedir.c common directory
3. Initialize a `hashtable` of 800 slots and call the `index(...)` function 
4. After successfully building the inverted-index structure, pass it to `indexFileGenerate()` in the `index.c` file.
    5.1 Iterates through the hashtable to write output
 * out to the index file
5. Clean up and exit with a zero status.

Pseudo code for `void index(const char* pageDirectory, index_t *wordMap)`:
1. For each document in the Crawler-generated directory:
    1.1 Extract the URL from the file and store a pointer to it
    1.2 Extract the depth from the file and store a pointer to it
    1.3 Extract the HTML content from the file and store a pointer to it
    1.4 Initialize a new webpage using this set of data
    1.5 Use `webpage_getNextWord(...)` to loop through each word in the webpage's HTML content:
        1.3.0 Normalize the word by making it lowercase.
        1.3.1 Check if the word is less than 3 characters
        1.3.2 If the word isn't in the `index`, initialize a new `counterset` for that word and add the pair
        1.3.3 If the word is in the `index`, simply increment its count  
2. Close the file pointer to the Crawler-directory's documents
3. Free the URL, depth, and HTML pointers
4. Free the webpage generated in step 1.4  

Data structures:
* `index_t` (via `index.h`)
  * a module to implement an abstract index_t type that represents an index in memory. It's a wrapper for hashtable_t
* `webpage_t` (via `webpage.h`)
  * contains member variables to hold its URL, depth, and HTML content
* `counters_t` (via `counters_new()`). Refer to `counters.h` for more information.
* `hashtable_t` (via `hashtable_new()`). Refer to `hashtable.h` for more information.

Persistent storage:
All output is written out to `indexFilename`.

Testing Plan
The indexer can be tested by:
reading from a crawler directory, writing a file.

Error handling and recovery:
* In all cases of failure, including memory allocation and sudden exits, we print out an error message and exit with a non-zero status.
