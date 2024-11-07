# Makefile for A6

WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR)

TESTFALGS = -DTEST_COUNTWORD # -DDEBUG

SRCS = main.c tree.c
OBJS = $(SRCS:%.c=%.o)

a6: $(OBJS)
	$(GCC) -O3 $(TESTFALGS) $(OBJS) -o a6 -lm

clean: # remove all machine generated files
	rm -f a5 *.o