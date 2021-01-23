#include <iostream>
#include <string>

#include "../include/acutest.h"
#include "../include/thread.hpp"
#include "../include/scheduler.hpp"


// template <typename T>
// std::string T::* treeNode<T>::keyValue = &unit_testing::id;

void insertNumber(void *arg) {
    void **argv = (void **)arg;
    conditionVariable *cond = (conditionVariable *)argv[0];
    int *doneCounter = (int *)argv[1];
    int position = *(int *)argv[2];
    int number = *(int *)argv[3];
    int *array = (int *)argv[4];

    array[position] = number;

    cond->lock();
    if (--(*doneCounter) != 0) {
        cond->unlock();
    }
    else {
        cond->unlock();
        cond->signal();
    }

    delete (int *)argv[2];
    delete (int *)argv[3];
    delete[] (void **)arg;
}

void scheduler_test(void) {
    scheduler sch(10);
    conditionVariable cond;
    int *array = new int[500];

    conditionVariable doneMutex;
    int doneCounter = 0;

    for (int i = 0; i < 500; i++) {
        void **args = new void *[8];
        args[0] = &doneMutex;
        args[1] = &doneCounter;
        args[2] = new int(i); //delete inside
        args[3] = new int(i * i); //delete inside
        args[4] = array;

        doneMutex.lock(); //pass task to scheduler
        sch.addTask(new task(insertNumber, (void *)args));
        doneCounter++;
        doneMutex.unlock();
    }
    //wait for all tasks to finish
    doneMutex.lock();
    while (doneCounter > 0) {
        doneMutex.wait();
    }
    doneMutex.unlock();

    for (int i = 0; i < 500; i+=17) {
        TEST_CHECK(array[i] == i*i);
    }

    // TEST_CHECK(array[0] == 0 && array[4] == 16 && array[50] == 2500 && array[100] == 10000 &&
    //         array[200] == 40000 && array[250] == 62500 && array[456] == 207936);

    delete[] array;
}

TEST_LIST = {
    {"Simple scheduler test" , scheduler_test},
	{ NULL, NULL }
};