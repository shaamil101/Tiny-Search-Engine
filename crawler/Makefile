L = ../libcs50
C = ../common

#OBJS = crawler.o pagedir.o file.o webpage.o mem.o bag.o hashtable.o set.o hash.o
OBJS = crawler.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
CC = gcc
LLIBS = $L/libcs50.a
CLIBS = $C/common.a

.PHONY: all test clean

all: crawler $L/libcs50.a $C/common.a

# executable depends on object files
crawler: $(OBJS) $(LLIBS) $(CLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(CLIBS) -o crawler

# dependencies: object files depend on header files
crawler.o: ./crawler.c $L/file.h $L/webpage.h $L/mem.h $C/pagedir.h $L/bag.h $L/hashtable.h $L/set.h $L/hash.h

test: 
	chmod +x testing.sh
	./testing.sh &> testing.out

clean:
	rm -f crawler
	rm -f *~ *.o
	rm -rf *.dSYM