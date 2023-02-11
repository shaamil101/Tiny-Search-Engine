
rm -rf test_data
mkdir test_data

# Test file according to the test case guidance
echo Testing without passing args
./indexer
echo Testing by passing one arg
./indexer test
echo Testing by passing three args
./indexer test test test
echo Testing by passing unvalid directory
./indexer ../crawler/test_data/test1/nothing fakedir 

echo test 0: Memory valgrind test with scrape depth 1
valgrind ./indexer ../crawler/test_data/test6/ ./test_data/test6_indexfile

echo test 1: letters test with depth 0
    ./indexer ../crawler/test_data/test1/ ./test_data/test1_indexFile
	cat ./test_data/test1_indexFile
	./indextest ./test_data/test1_indexFile ./test_data/test1_indexFileNew
	cat ./test_data/test1_indexFileNew

echo test 2: letters test with depth 1
    ./indexer ../crawler/test_data/test2/ ./test_data/test2_indexFile
	cat ./test_data/test2_indexFile
	./indextest ./test_data/test2_indexFile ./test_data/test2_indexFileNew
	cat ./test_data/test2_indexFileNew

echo test 3: letters test with depth 2
	./indexer ../crawler/test_data/test3/ ./test_data/test3_indexFile
	cat ./test_data/test3_indexFile
	./indextest ./test_data/test3_indexFile ./test_data/test3_indexFileNew
	cat ./test_data/test3_indexFileNew

echo test 4: letters test with depth 10
	./indexer ../crawler/test_data/test4/ ./test_data/test4_indexFile
	cat ./test_data/test4_indexFile
	./indextest ./test_data/test4_indexFile ./test_data/test4_indexFileNew
	cat ./test_data/test4_indexFileNew

echo test 5: Test with to scrape, depth 0
	./indexer ../crawler/test_data/test5/ ./test_data/test5_indexFile
	cat ./test_data/test5_indexFile
	./indextest ./test_data/test5_indexFile ./test_data/test5_indexFileNew
	cat ./test_data/test5_indexFileNew
echo test 6: Test with to scrape, depth 1
	./indexer ../crawler/test_data/test6/ ./test_data/test6_indexFile
	cat ./test_data/test6_indexFile
	./indextest ./test_data/test6_indexFile ./test_data/test6_indexFileNew
	cat ./test_data/test6_indexFileNew
echo test 7: Test with wikipedia, depth 0
	./indexer ../crawler/test_data/test7/ ./test_data/test7_indexFile
	cat ./test_data/test7_indexFile
	./indextest ./test_data/test7_indexFile ./test_data/test7_indexFileNew
	cat ./test_data/test7_indexFileNew
