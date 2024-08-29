# Makefile for A1

WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR) 

TESTFALGS = #-DTEST_COUNTWORD -DDEBUG

OBJS = main.c

a1: $(OBJS)
	$(GCC) $(TESTFALGS) $(OBJS) -o a1

clean: # remove all machine generated files
	rm -f a1 *.o