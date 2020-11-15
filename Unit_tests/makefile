CC=g++
CFLAGS=-std=c++11 -Wall -g3

list_test: ../include/list.hpp list_test.cpp
	$(CC) $(CFLAGS) list_test.cpp -o list_test

example: example.o utilities.o spec.o
	$(CC) $(CFLAGS) ../builder/example.o  ../builder/spec.o  ../builder/utilities.o -o example

spec.o: ../include/spec.cpp
	$(CC) $(CFLAGS) -c ../include/spec.cpp -o ../builder/spec.o

utilities.o: ../include/utilities.cpp
	$(CC) $(CFLAGS) -c ../include/utilities.cpp -o ../builder/utilities.o

example.o: example.cpp
	$(CC) $(CFLAGS) -c example.cpp -o ../builder/example.o

clean:
	rm -rf example ../builder/* list_test