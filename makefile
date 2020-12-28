#old makefile, to be changed
CC=g++
CFLAGS=-std=c++11 -Wall -g3

main: utilities.o spec.o jsonParser.o tf_idf.o matrix.o logistic_regression.o
	$(CC) $(CFLAGS) main.cpp ./builder/utilities.o ./builder/spec.o ./builder/jsonParser.o ./builder/tf_idf.o ./builder/matrix.o ./builder/logistic_regression.o -o master.out

utilities.o: ./include/utilities.cpp ./include/utilities.hpp
	$(CC) $(CFLAGS) -c ./include/utilities.cpp -o ./builder/utilities.o

spec.o: ./include/spec.cpp ./include/spec.hpp
	$(CC) $(CFLAGS) -c ./include/spec.cpp -o ./builder/spec.o

jsonParser.o: ./include/jsonParser.cpp ./include/jsonParser.hpp
	$(CC) $(CFLAGS) -c ./include/jsonParser.cpp -o ./builder/jsonParser.o

tf_idf.o: ./include/tf_idf.cpp ./include/tf_idf.hpp ./include/list.hpp ./include/avl.hpp
	$(CC) $(CFLAGS) -c ./include/tf_idf.cpp -o ./builder/tf_idf.o

matrix.o: ./include/matrix.cpp ./include/matrix.hpp
	$(CC) $(CFLAGS) -c ./include/matrix.cpp -o ./builder/matrix.o

logistic_regression.o: ./include/logistic_regression.cpp ./include/logistic_regression.hpp
	$(CC) $(CFLAGS) -c ./include/logistic_regression.cpp -o ./builder/logistic_regression.o

jsonmain: utilities.o spec.o jsonParser.o tf_idf.o
	$(CC) $(CFLAGS) jsonmain.cpp ./builder/utilities.o ./builder/spec.o ./builder/jsonParser.o ./builder/tf_idf.o -o jsonmain.out

lrmain: utilities.o spec.o jsonParser.o tf_idf.o matrix.o logistic_regression.o 
	$(CC) $(CFLAGS) lrmain.cpp ./builder/*.o -o lrmain.out

clean:
	rm -rf master.out ./builder/*
