#include <iostream>

#include "include/thread.hpp"
#include "include/scheduler.hpp"

void foo(void *argv) {
    std::cout << (char *)argv << std::endl;
}


int main() {

    std::cout << "Before" << std::endl;

    const char *test1 = "This is a test";
    const char *test2 = "This is another test";
    const char *test3 = "This is the final test";

    std::cout << "Creating threads" << std::endl;
    scheduler sch(3);
    std::cout << "Created threads" << std::endl;
    list<task> l;
    std::cout << "Creating tasks" << std::endl;
    task *temp = new task(foo, (void *)test1);
    sch.addTask(temp);
    std::cout << "Created tasks" << std::endl;


    return 0;
}