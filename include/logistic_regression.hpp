#ifndef LOGISTIC_REGRESSION_HPP
#define LOGISTIC_REGRESSION_HPP

#include <iostream>
#include "matrix.hpp"
// #include <cmath>

class logistic_regression {
private:
    float learningRate;
    matrix w;
    matrix b;

    matrix *predictions;
    // matrix j;

    static void sigmoid(matrix &v);
    static void sigmoid(matrix *v);
    // static float acusigmoid(float v);
public:
    static float abs(float v);
    // logistic_regression() {}; //for loading from file only;
    logistic_regression(float learningRate, int rows, int columns);
    ~logistic_regression();

    matrix *getWeights();

    matrix *gradient(matrix &vectors, matrix &prediction, int *y);
    matrix *predict(matrix &vectors);
    float epoch(matrix &vectors, int *y);
    // float cost(matrix values, int y);

};

#endif /*LOGISTIC_REGRESSION_HPP*/