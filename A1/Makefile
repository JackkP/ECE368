# Makefile for A1

WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR) 

TESTFALGS = -DDEBUG

SRCS = main.c
OBJS = $(SRCS:%.c=%.o)

A1: $(OBJS) 
	$(GCC) $(TESTFALGS) $(OBJS) -o A1

.c.o: 
	$(GCC) $(TESTFALGS) -c $*.c 

lean: # remove all machine generated files
	rm -f hw12 *.o output?

