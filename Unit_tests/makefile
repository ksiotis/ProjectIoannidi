CC=g++
CFLAGS=-std=c++11 -Wall -g3

hashtable_test: ../include/hashtable.hpp hashtable_test.cpp utilities.o
    $(CC) $(CFLAGS) hashtable_test.cpp ../builder/utilities.o -o hashtable_test

avl_test: ../include/avl.hpp avl_test.cpp utilities.o
	$(CC) $(CFLAGS) avl_test.cpp ../builder/utilities.o -o avl_test

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
	rm -rf example ../builder/* list_test avl_test hashtable_test