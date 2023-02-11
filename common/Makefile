OBJS = pagedir.o index.o word.o ../libcs50/file.o ../libcs50/webpage.o ../libcs50/mem.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

# build $(LIB) by archiving object files
$(LIB): $(OBJS)
	ar cr $@ $^

# dependencies: object files depend on header files
pagedir.o: pagedir.h ../libcs50/webpage.h ../libcs50/file.h
word.o : word.h
index.o : index.h ../libcs50/webpage.h ../libcs50/file.h ../libcs50/hashtable.h ../libcs50/counters.h 
../libcs50/file.o: ../libcs50/file.h
../libcs50/webpage.o: ../libcs50/file.h ../libcs50/webpage.h ../libcs50/mem.h
../libcs50/mem.o: ../libcs50/mem.h

.PHONY: all clean

clean:
	rm -f core
	rm -f $(LIB) *~ *.o