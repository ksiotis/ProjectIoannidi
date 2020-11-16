#old makefile, to be changed
CC=g++
CFLAGS=-std=c++11 -Wall -g3

main: utilities.o spec.o jsonParser.o
	$(CC) $(CFLAGS) main.cpp ./builder/utilities.o ./builder/spec.o ./builder/jsonParser.o -o master.out

utilities.o: ./include/utilities.cpp
	$(CC) $(CFLAGS) -c ./include/utilities.cpp -o ./builder/utilities.o

spec.o: ./include/spec.cpp
	$(CC) $(CFLAGS) -c ./include/spec.cpp -o ./builder/spec.o

jsonParser.o: ./include/jsonParser.cpp
	$(CC) $(CFLAGS) -c ./include/jsonParser.cpp -o ./builder/jsonParser.o

jsonmain: utilities.o spec.o jsonParser.o
	$(CC) $(CFLAGS) jsonmain.cpp ./builder/utilities.o ./builder/spec.o ./builder/jsonParser.o -o jsonmain.out

clean:
	rm -rf master.out ./builder/*
