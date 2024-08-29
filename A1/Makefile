# Makefile for A1

WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR) 

SRC = main.c

a1: $(SRC)
	$(GCC) $(SRC) -o a1

clean: # remove all machine generated files
	rm -f a1 *.o