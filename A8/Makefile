# Makefile for A1

WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c11 -g $(WARNING) $(ERROR) 

SRC = main.c

a8: $(SRC)
	$(GCC) $(SRC) -o a8

clean: # remove all machine generated files
	rm -f a8 *.o