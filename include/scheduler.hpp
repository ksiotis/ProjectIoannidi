#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <iostream>

#include "list.hpp"
#include "thread.hpp"

class task {
/*  contains pointer to function:
        must return void
    arguments for said funtion:
        must be pre-malloced or NULL, gets freed in destructor*/
private:
    void (*function_pointer)(void *);
    void *arguments;
public:
    task(void (*function_pointer)(void *), void *arguments, void *target = NULL);
    ~task();

    void run();
};


class scheduler {
public:
    int execution_threads;
    thread **pool;
    conditionVariable cond;
    bool running;
    semaphore runningMutex;
    
    list<task> q;
    scheduler(int threads);
    ~scheduler();

    bool getRunning();
    int getCount();

    void addTask(task *myValue);
    task *popTask();
};

void *threadMain(scheduler &sch, void *argv);

#endif /*SCHEDULER_HPP*/