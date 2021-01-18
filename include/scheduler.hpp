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
private:
    int execution_threads;
    thread **pool;
    conditionVariable cond;
    bool running;
    
    list<task> q;
public:
    scheduler(int threads);
    ~scheduler();

    void addTask(task *myValue);
    task *popTask();

    void *threadMain(void *argv);
};

#endif /*SCHEDULER_HPP*/