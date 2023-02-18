L = ../libcs50
C = ../common

#OBJS = querier.o pagedir.o file.o webpage.o mem.o bag.o hashtable.o set.o hash.o
OBJS = querier.o
PROGS = querier
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
CC = gcc
LLIBS = $L/libcs50.a
CLIBS = $C/common.a

.PHONY: all test clean

all: $(PROGS)

# executable depends on object files
querier: $(OBJS) $(CLIBS) $(LLIBS) 
	$(CC) $(CFLAGS) $(OBJS) $(CLIBS) $(LLIBS) -o querier

# dependencies: object files depend on header files
querier.o: ./querier.c $L/file.h $L/webpage.h $L/mem.h $C/pagedir.h $L/bag.h $L/hashtable.h $L/set.h $L/hash.h

test: 
	chmod +x testing.sh
	bash -v testing.sh
	./testing.sh &> testing.out

clean:
	rm -f querier
	rm -f *~ *.o
	rm -rf *.dSYM