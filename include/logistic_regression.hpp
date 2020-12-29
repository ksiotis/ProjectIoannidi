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
    matrix *gradient(matrix &vectors, matrix &prediction, int *y);
    float cost(int *y);
public:
    static float abs(float v);
    static float compare(matrix &values, int *actual_values);
    // logistic_regression() {}; //for loading from file only;
    logistic_regression(float learningRate, int columns, bool random=true);
    ~logistic_regression();

    matrix *getWeights();
    matrix *getBias();

    float epoch(matrix &vectors, int *y);
    matrix *predict(matrix &vectors);

    static logistic_regression *loadModel(char *filename);
    void extractModel(char *filename);
};

#endif /*LOGISTIC_REGRESSION_HPP*/