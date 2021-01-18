#include <pthread.h>
#include <iostream>

// #include "scheduler.hpp"
#include "thread.hpp"

thread::thread(void *(scheduler::*function)(void *), void *argv) {
    pthread_create(&thread_id, NULL, (void *(*)(void *))function, argv);
}

thread::~thread() {
    std::cout << "Ending " << thread_id << std::endl;
    pthread_join(thread_id, NULL);
}

//~~~~~semaphores~~~~~

semaphore::semaphore() {
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        //TODO error
    }
}

semaphore::~semaphore() {
    if (pthread_mutex_destroy(&mutex) != 0) {
        //TODO error
    }
}

pthread_mutex_t *semaphore::getMutexP() { return &mutex; }

int semaphore::lock() {
    return pthread_mutex_lock(&mutex);
}

int semaphore::unlock() {
    return pthread_mutex_unlock(&mutex);
}

//~~~~~conditionVariable~~~~~

conditionVariable::conditionVariable() {
    if (pthread_cond_init(&cond, NULL) != 0) {
        //TODO error
    }
}

conditionVariable::~conditionVariable() {
    pthread_cond_destroy(&cond);
}

void conditionVariable::lock() { mutex.lock(); }

void conditionVariable::unlock() { mutex.unlock(); }

void conditionVariable::wait() { pthread_cond_wait(&cond, mutex.getMutexP()); }

void conditionVariable::signal() { pthread_cond_signal(&cond); }

void conditionVariable::broadcast() { pthread_cond_broadcast(&cond); }
