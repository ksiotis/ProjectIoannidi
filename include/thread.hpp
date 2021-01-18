#ifndef THREAD_HPP
#define THREAD_HPP

class scheduler;

class thread {
private:
    pthread_t thread_id;
public:
    thread(void *(scheduler::*function_pointer)(void *), void *argv);
    ~thread();
};

class semaphore {
private:
    pthread_mutex_t mutex;

public:
    semaphore();
    ~semaphore();

    pthread_mutex_t *getMutexP();

    int lock();
    int unlock();
};

class conditionVariable {
private:
    semaphore mutex;
    pthread_cond_t cond;
public:
    conditionVariable();
    ~conditionVariable();

    void lock();
    void unlock();

    void wait();
    void signal();
    void broadcast();
};

#endif /*THREAD_HPP*/