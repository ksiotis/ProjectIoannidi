#old makefile, to be changed
CC=g++
CFLAGS=-std=c++11 -Wall -g3

test: test.o
	$(CC) $(CFLAGS) test.o -o test

test.o: test.cpp
	$(CC) $(CFLAGS) -c test.cpp

clean:
	rm -rf test test.o
