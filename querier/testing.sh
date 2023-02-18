#!/bin/bash

# This file contains normal and edge test cases for querier.c

# I'm making use of the crawler test with to scrape, depth 1 output and
# the index file of the output loaded into querier as an inverted-index hashtable.

# I've also included a list of words and queries in testquery.txt to test various queries

# wikipedia 1 6 
# design 1 1 
# amp 1 1 
# list 1 1 
# middot 1 6 
# other 1 1 
# kauler 1 1 
# locally 1 1 
# computer 1 1 
# dartmouth 1 1 
# index 1 3 
# hash 1 1 
# open 1 1 
# science 1 1 
# web 1 1 
# available 1 1 
# made 1 1 
# unix 1 1 
# the 1 6 
# table 1 1 
# offline 1 1 
# local 1 1 
# and 1 1 
# linked 1 1 
# language 1 1 
# website 1 3 
# college 1 1 
# sites 1 1 
# mirror 1 1 
# httrack 1 3 
# leto 1 1 
# browser 1 1 
# contributors 1 1 
# free 1 6 
# programming 1 1 
# encyclopedia 1 6 
# roche 1 1 
# source 1 1 
# copier 1 3 
# copy 1 1 
# xavier 1 1 


# zero arguments
./querier
# one argument
./querier fakearg1
# three arguments
./querier fakearg1 fakearg2 fakearg3
# two arguments but with a non-existent pageDirectory
./querier ../DNE ../indexer/test_data/test6_indexfile
# two arguments but with a non-existent indexFilename
./querier ../crawler/test_data/test6 ../indexer/DNE
# testing querier with testingInput
./querier ../crawler/test_data/test6 ../indexer/test_data/test6_indexfile < testquery.txt
