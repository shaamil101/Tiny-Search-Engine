L = ../libcs50
C = ../common

#OBJS = indexer.o pagedir.o file.o webpage.o mem.o bag.o hashtable.o set.o hash.o
OBJS = indexer.o
PROGS = indexer indextest
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
CC = gcc
LLIBS = $L/libcs50.a
CLIBS = $C/common.a

.PHONY: all test clean

all: $(PROGS)

# executable depends on object files
indexer: $(OBJS) $(CLIBS) $(LLIBS) 
	$(CC) $(CFLAGS) $(OBJS) $(CLIBS) $(LLIBS) -o indexer
indextest: indextest.o $(CLIBS) $(LLIBS) 
	$(CC) $(CFLAGS) indextest.o $(CLIBS) $(LLIBS) -o indextest
# dependencies: object files depend on header files
indexer.o: ./indexer.c $L/file.h $L/webpage.h $L/mem.h $C/pagedir.h $L/bag.h $L/hashtable.h $L/set.h $L/hash.h
indextest.o: ./indextest.c $L/file.h $L/webpage.h $L/mem.h $C/pagedir.h $L/bag.h $L/hashtable.h $L/set.h $L/hash.h
test: 
	chmod +x testing.sh
	./testing.sh &> testing.out

clean:
	rm -f indexer
	rm -f indextest
	rm -f *~ *.o
	rm -rf *.dSYM