#include <iostream>
#include <pthread.h> 

#include "list.hpp"
#include "thread.hpp"
#include "scheduler.hpp"

//~~~~~task~~~~~

task::task(void (*foo)(void *), void *argv, void *target) :
    function_pointer(foo),
    arguments(argv) {
}

task::~task() {
    if (arguments != NULL)
        free(arguments);
}

void task::run() {
    function_pointer(arguments);
}

//~~~~~scheduler~~~~~

scheduler::scheduler(int nthreads) :
    execution_threads(nthreads),
    cond(),
    running(true) {
    pool = new thread*[execution_threads];
    for (int i = 0; i < execution_threads; i++) {
        std::cout << "Creating threads" << std::endl;
        pool[i] = new thread(&scheduler::threadMain), (void *)NULL);
        std::cout << "Created thread " << pool[i];
    }
}

scheduler::~scheduler() {
    q.emptyList(true);
}

void scheduler::addTask(task *myValue) {
    cond.lock();
    q.insert(myValue);
    cond.unlock();
    cond.signal();
}

task *scheduler::popTask() {
    cond.lock();
    task *returnable = q.removeStart();
    cond.unlock();
    return returnable;
}

void *scheduler::threadMain(void *argv) {
    while (running) {

        //condition variable waiting
        cond.lock();
        while (!q.getCount()) {
            cond.wait();
        }

        task *currentTask = popTask();

        cond.unlock();

        //run Task
        currentTask->run();
        delete currentTask;

    }
}
