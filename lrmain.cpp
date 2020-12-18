#include <iostream>
#include "include/matrix.hpp"
#include "include/logistic_regression.hpp"

matrix *vector(float a, float b, float c) {
    matrix *ret = new matrix(1,3);
    ret->table[0][0] = a;
    ret->table[0][1] = b;
    ret->table[0][2] = c;

    return ret;
}

int main() {
    logistic_regression lr(10.0f, 1, 3);


    matrix *temp = vector(1, 2, 3);
    std::cout << lr.epoch(*temp, 1) << std::endl;
    delete temp;
    temp = vector(10, 20, 30);
    std::cout << lr.epoch(*temp, 0) << std::endl;
    // lr.epoch(*temp, 0);
    delete temp;
    temp = vector(0, 1, 0);
    std::cout << lr.epoch(*temp, 1) << std::endl;
    // lr.epoch(*temp, 1);
    delete temp;
    temp = vector(100, 2, 10);
    std::cout << lr.epoch(*temp, 0) << std::endl;
    delete temp;
    // lr.epoch(*temp, 0);

    std::cout << (float)(lr.predict(*vector(100,30,90))) << std::endl;
}