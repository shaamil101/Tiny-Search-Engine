
rm -rf test_data
mkdir test_data

# Test file according to the test case guidance
echo Testing without passing args
./crawler
echo Testing by passing wrong args
./crawler hello my name is christina
echo Testing by passing wrong url
./crawler http://www.cs.dartmouth.edu/ ./test_data/ 0
echo Testing by passing wrong directory
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ fakedir 0
echo Testing by passing invalid depth
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ./test_data/ -5

echo test 0: Memory valgrind test
mkdir test_data/test0/
valgrind ./crawler HTTP://cs50tse.cs.dartmouth.edu/tse/toscrape/ ./test_data/test0/ 1

echo test 1: letters test with depth 0
mkdir test_data/test1/
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ./test_data/test1/ 0

echo test 2: letters test with depth 1
mkdir test_data/test2/
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ./test_data/test2/ 1

echo test 3: letters test with depth 2
mkdir test_data/test3/
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ./test_data/test3/ 2

echo test 4: letters test with depth 10
mkdir test_data/test4/
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ./test_data/test4/ 10

echo test 5: Test with to scrape, depth 0
mkdir test_data/test5/
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ./test_data/test5/ 0

echo test 6: Test with to scrape, depth 1
mkdir test_data/test6/
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ./test_data/test6/ 1

echo test 7: wikipedia, depth 0
mkdir test_data/test7/
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ./test_data/test7/ 0

echo test 8: wikipedia, depth 1
mkdir test_data/test8/
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ./test_data/test8/ 1