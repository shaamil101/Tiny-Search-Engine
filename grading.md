# Lab 4

## Student: Shaamil Shaw Alem

## Grader: Jessie Li

Recall the [Assignment](https://github.com/cs50spring2021/tse-labs/tree/main/crawler).

**Total: 98.5/100 points**

## (10) Overall

10/10 points

## (10) Makefiles

10/10 points

* successfully compiles all programs from the top-level directory, but compiling within each folder leads to `make` errors
* if you want to use `libcs50-given.a`, modify the libcs50 Makefile: 

```bash
# $(LIB): $(OBJS)
#	ar cr $(LIB) $(OBJS)

$(LIB): libcs50-given.a
	cp libcs50-given.a $(LIB)
```
* in the crawler Makefile, change `$L/libcs50.a` to `$L/libcs50-given.a`

## (3) Documentation

2.5/3 points

* -0.5 missing a README in common directory
* should include `REQUIREMENTS.md`, `DESIGN.md`, and `IMPLEMENTATION.md` in crawler directory

## (12) Testing

12/12 points

## (20) Coding Style

20/20 points

## (35) Functionality

34/35 points

* -1 crawler still runs when invalid maxDepth=1b is passed as an argument; should exit

## (10) Memory leaks

10/10 points
