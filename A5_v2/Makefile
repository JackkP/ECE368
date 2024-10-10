# Makefile for A5

WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c11 -g $(WARNING) $(ERROR)

TESTFALGS = -DTEST_COUNTWORD # -DDEBUG

SRCS = main.c tree.c
OBJS = $(SRCS:%.c=%.o)

a5: $(OBJS)
	$(GCC) $(TESTFALGS) $(OBJS) -o a5 -lm

clean: # remove all machine generated files
	rm -f a5 *.o