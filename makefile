#old makefile, to be changed
CC=g++
CFLAGS=-std=c++11 -Wall -g3

test: test.o utilities.o
	$(CC) $(CFLAGS) test.o utilities.o -o test

test.o: test.cpp
	$(CC) $(CFLAGS) -c test.cpp

utilities.o: ./include/utilities.cpp
	$(CC) $(CFLAGS) -c ./include/utilities.cpp -o utilities.o

clean:
	rm -rf test test.o utilities.o
