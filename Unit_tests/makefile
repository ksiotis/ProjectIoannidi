CC=g++
CFLAGS=-std=c++11 -Wall -g3 -pthread

unit_test: spec_test jsonParser_test hashtable_test avl_test list_test matrix_test scheduler_test

spec_test: ../include/list.hpp ../include/spec.hpp spec_test.cpp spec.o utilities.o
	$(CC) $(CFLAGS) spec_test.cpp ../builder/spec.o ../builder/utilities.o -o spec_test

jsonParser_test: jsonParser.o utilities.o
	$(CC) $(CFLAGS) jsonParser_test.cpp ../builder/utilities.o ../builder/jsonParser.o -o jsonParser_test

hashtable_test: ../include/hashtable.hpp hashtable_test.cpp utilities.o
	$(CC) $(CFLAGS) hashtable_test.cpp ../builder/utilities.o -o hashtable_test

avl_test: ../include/avl.hpp ./avl_test.cpp utilities.o
	$(CC) $(CFLAGS) avl_test.cpp ../builder/utilities.o -o avl_test

list_test: ../include/list.hpp list_test.cpp
	$(CC) $(CFLAGS) list_test.cpp -o list_test

matrix_test: matrix_test.cpp scheduler.o thread.o matrix.o
	$(CC) $(CFLAGS) matrix_test.cpp ../builder/scheduler.o ../builder/thread.o ../builder/matrix.o -o matrix_test

tf_idf_test: tf_idf_test.cpp tf_idf.o jsonParser.o utilities.o
	$(CC) $(CFLAGS) tf_idf_test.cpp ../builder/jsonParser.o ../builder/utilities.o ../builder/tf_idf.o -o tf_idf_test

scheduler_test: thread.o scheduler.o
	$(CC) $(CFLAGS) scheduler_test.cpp ../builder/thread.o ../builder/scheduler.o -o scheduler_test

spec.o: ../include/spec.cpp
	$(CC) $(CFLAGS) -c ../include/spec.cpp -o ../builder/spec.o

utilities.o: ../include/utilities.cpp
	$(CC) $(CFLAGS) -c ../include/utilities.cpp -o ../builder/utilities.o

jsonParser.o: ../include/jsonParser.cpp ../include/jsonParser.hpp 
	$(CC) $(CFLAGS) -c ../include/jsonParser.cpp -o ../builder/jsonParser.o

matrix.o: ../include/matrix.cpp ../include/matrix.hpp
	$(CC) $(CFLAGS) -c ../include/matrix.cpp -o ../builder/matrix.o

logistic_regression.o: matrix.o ../include/logistic_regression.cpp
	$(CC) $(CFLAGS) -c ../include/logistic_regression.cpp -o ../builder/logistic_regression.o

thread.o: ../include/thread.cpp ../include/thread.hpp
	$(CC) $(CFLAGS) -c ../include/thread.cpp -o ../builder/thread.o

scheduler.o: thread.o ../include/scheduler.cpp ../include/scheduler.hpp
	$(CC) $(CFLAGS) -c ../include/scheduler.cpp -o ../builder/scheduler.o

example.o: example.cpp
	$(CC) $(CFLAGS) -c example.cpp -o ../builder/example.o

clean:
	rm -rf list_test avl_test hashtable_test spec_test jsonParser_test matrix_test logistic_regression_test tf_idf_test scheduler_test thread_test
