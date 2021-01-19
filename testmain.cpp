#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
// #include <unistd.h>

#include "include/thread.hpp"
#include "include/scheduler.hpp"

// semaphore printMutex;
void foo(void *argv) {
    // printMutex.lock();    
    for(int i=0; i<=100000000; i++);
    int mine = *(int *)argv;

    std::cout << log10(mine*mine <= 0 ? 1 : mine * mine) * log10(mine/10 <= 0 ? 1 : mine/10) << std::endl;
    // printMutex.unlock();
}


int main() {
    std::string str = "This is string #";

    // char *currentString;
    scheduler sch(16);

    for (int i = 0; i < 1000; i++) {
        std::string currentString = str + std::to_string(i);

        void *currentCString = (void *)malloc(sizeof(int));
        *(int *)currentCString = i;

        sch.addTask(new task(foo, currentCString));
    }

    return 0;
}