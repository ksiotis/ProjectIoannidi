#ifndef LOGISTIC_REGRESSION_HPP
#define LOGISTIC_REGRESSION_HPP

#include <iostream>
#include "matrix.hpp"
#include "thread.hpp"
// #include <cmath>

class logistic_regression {
private:
    float learningRate;
    matrix w;
    matrix b;

    matrix *predictions;
    // matrix j;

    // static float acusigmoid(float v);
    matrix *gradient(matrix &vectors, matrix &prediction, int *y);
    matrix *gradient(matrix &vectors, matrix &prediction, int *y, scheduler &sch);
public:
    static void sigmoid(matrix &v);
    static void sigmoid(matrix &v, scheduler &sch);
    static void sigmoid(matrix *v);
    static void sigmoid(matrix *v, scheduler &sch);
    
    static float abs(float v);
    static float compare(matrix &values, int *actual_values);
    static float compare(matrix &values, int *actual_values, scheduler &sch);
    static float accuracy(matrix &values, int *actual_values);
    static float accuracy(matrix &values, int *actual_values, scheduler &sch);
    // logistic_regression() {}; //for loading from file only;
    logistic_regression(float learningRate, int columns, bool random=true);
    ~logistic_regression();

    matrix *getWeights();
    matrix *getBias();

    void epoch(matrix &vectors, int *y);
    void epoch(matrix &vectors, int *y, scheduler &sch);
    matrix *predict(matrix &vectors);
    matrix *predict(matrix &vectors, scheduler &sch);

    static logistic_regression *loadModel(const char *filename);
    void extractModel(const char *filename);
};

#endif /*LOGISTIC_REGRESSION_HPP*/