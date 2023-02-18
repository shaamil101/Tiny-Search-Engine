# CS50 Winter 2021
## Shaamil Karim Shaw Alem 

### querier

The TSE Querier is a standalone program that makes use of the output from TSE Crawler and the TSE Querier and answers search queries submitted via stdin.

### Functionality

All of the functionality has been implemented

### Files

* `querier.c` - querier implementation
* `DESIGN.md` -  design doc
* `IMPLEMENTATION.md` - implementation explanation doc
* `REQUIREMENTS.md` - requirement description
* `Makefile` - compilation procedure
* `testing.sh` -  testing script
* `testing.out` -  output from testing.sh
* `testquery.txt` - feeder input file

### Assumptions

* pageDirectory is not longer than 500 characters
* The index file pathway is not longer than 100 characters
* 800 slots are sufficient for the inverted-index hashtable


### Compilation

To compile, simply `make`.

### Testing

I have made use of an extra file `testquery.txt` with queries to test the querier from testing.sh. The output is outputted to testing.out. Valgrind can be run and then the stdin can be exited from to check that there are no memory leaks. 
