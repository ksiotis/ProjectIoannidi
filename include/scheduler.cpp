#include <iostream>
#include <pthread.h> 

#include "list.hpp"
#include "thread.hpp"
#include "scheduler.hpp"

//~~~~~task~~~~~

task::task(void (*foo)(void *), void *argv) :
    function_pointer(foo),
    arguments(argv) {
}

task::~task() {}

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
        pool[i] = new thread((void *(*)(void *))&threadMain, (void *)this);
    }
}

scheduler::~scheduler() {
    runningMutex.lock();
    running = false;
    runningMutex.unlock();

    std::cout << "Broadcasting..." << std::endl;
    cond.broadcast();
    for (int i = 0; i < execution_threads; i++) {
        // std::cout << "waiting for " << pool[i] << ": " << std::flush;
        delete pool[i];
        // std::cout << "done" << std::endl;
    }
    delete[] pool;
    q.emptyList(true);
    std::cout << "done" << std::endl;
}

bool scheduler::getRunning() {
    runningMutex.lock();
    bool ret = running;
    runningMutex.unlock();

    return ret;
}

int scheduler::getCount() {
    cond.lock();
    int ret = q.getCount();
    cond.unlock();

    return ret;
}

void scheduler::addTask(task *myValue) {
    cond.lock();
    q.insert(myValue);
    cond.unlock();

    cond.signal();
}

task *scheduler::popTask() {
    task *returnable = q.removeStart();
    return returnable;
}

void *threadMain(scheduler &sch, void *argv) {
    while (1) {
        if (sch.getRunning() == 0 && sch.q.getCount() == 0) { //if it must end

            break; //end
        }
        
        //condition variable waiting
        sch.cond.lock();
        while (sch.getRunning() && sch.q.getCount() == 0) { //wait while queue is empty
            sch.cond.wait();
        }
        task *currentTask = sch.popTask();
        sch.cond.unlock();

        //run Task
        if (currentTask != NULL) {
            currentTask->run();
            delete currentTask;
        }

    }
    return NULL;
}
