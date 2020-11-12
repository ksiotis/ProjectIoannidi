#old makefile, to be changed
CC=g++
CFLAGS=-std=c++11 -Wall -g3

test: test.o utilities.o spec.o
	$(CC) $(CFLAGS) ./builder/test.o ./builder/utilities.o ./builder/spec.o -o test

test2: test2.o utilities.o spec.o
	$(CC) $(CFLAGS) ./builder/test.o ./builder/utilities.o -o test

test.o: test.cpp
	$(CC) $(CFLAGS) -c test.cpp -o ./builder/test.o

test2.o: test2.cpp
	$(CC) $(CFLAGS) -c test2.cpp ./builder/test.o

utilities.o: ./include/utilities.cpp
	$(CC) $(CFLAGS) -c ./include/utilities.cpp -o ./builder/utilities.o

spec.o: ./include/spec.cpp
	$(CC) $(CFLAGS) -c ./include/spec.cpp -o ./builder/spec.o

clean:
	rm -rf test test2 ./builder/*
