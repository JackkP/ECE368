# Makefile for A1

WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c11 -g $(WARNING) $(ERROR) 

TESTFALGS = -DTEST_COUNTWORD # -DDEBUG

SRCS = list.c main.c
OBJS = $(SRCS:%.c=%.o)

a4: $(OBJS)
	$(GCC) $(TESTFALGS) $(OBJS) -o a4

clean: # remove all machine generated files
	rm -f a4 *.o