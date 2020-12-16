#ifndef LOGISTIC_REGRESSION_HPP
#define LOGISTIC_REGRESSION_HPP

#include <iostream>
// #include <cmath>
#include "matrix.hpp"

class logistic_regression {
private:
    float learningRate;
    matrix w;
    matrix b;

    matrix j;

    static float sigmoid(float v);
    // static float acusigmoid(float v);
public:
    // logistic_regression() {}; //for loading from file only;
    logistic_regression(float learningRate, int rows, int columns);
    ~logistic_regression();

    float train(matrix values);
    float predict(matrix values);
};

float logistic_regression::sigmoid(float x) {
    /*aproximate sigmoid function, faster but less accurate*/
    return 0.5f * x / (1 + abs(x)) + 0.5f;
}

// float logistic_regression::sigmoid(float x) {
//     /*aproximate sigmoid function, faster but less accurate*/
//     return 1 / exp(-x);
// }

logistic_regression::logistic_regression(float learningRate, int rows, int columns):
    learningRate(learningRate),
    w(rows, columns),
    b(rows, 1),
    j(1, columns) 
{   
    for (int j = 0; j < columns; j++) {
        for (int i = 0; i < rows; i++) {
            w.table[i][j] = rand()/RAND_MAX;
        }
        b.table[1][j] = rand()/RAND_MAX;
    }
}

logistic_regression::~logistic_regression() {}


#endif /*LOGISTIC_REGRESSION_HPP*/