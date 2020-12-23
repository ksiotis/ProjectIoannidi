#include <iostream>
// #include <cmath>
#include "matrix.hpp"
#include "logistic_regression.hpp"

float logistic_regression::abs(float v) {
    return v < 0 ? -v : v;
}

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
    b(rows, 1)
    // j(1, columns) 
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            w.table[i][j] = (float)(rand())/RAND_MAX;
        }
        b.table[i][0] = (float)(rand())/RAND_MAX;
    }
}

logistic_regression::~logistic_regression() {
    std::cout << "Deleting the following matrices:" << std::endl;
    std::cout << "w:" << std::endl;
    w.print();
    std::cout << "b:" << std::endl;
    b.print();
    // std::cout << "j:" << std::endl;
    // j.print();
}

matrix *logistic_regression::getWeights() { return &w; }

matrix *logistic_regression::gradient(matrix &vectors, float predictions, int y) {
    if (vectors.getColumns() != w.getColumns()) {
        std::cerr << "Invalid number of elements" << std::endl;
        // return;
    }
    int rows = vectors.getRows();
    int columns = vectors.getColumns();

    // std::cout << rows << "  " << columns + 1 << std::endl;
    matrix *thetas = new matrix(rows, columns + 1); //extra position for b
    
    //rows = 1, //TODO expand for multiple rows => able to do batch

    for (int i = 0; i < rows; i++) {
        float error = predictions - y;
        for (int j = 0; j < columns; j++) {
            thetas->table[0][j] += error * vectors.table[i][j];
        }
        thetas->table[i][columns] += error;
    }
    return thetas;
}

float logistic_regression::predict(matrix &vectors) {
    if (vectors.getColumns() != w.getColumns()) {
        std::cerr << "Invalid number of elements" << std::endl;
        // return -1;
    }
    float temp = matrix::dot(vectors, w) + b.table[0][0];
    return sigmoid(temp);
}

float logistic_regression::epoch(matrix &vectors, int y) {
    if (vectors.getColumns() != w.getColumns()) {
        std::cerr << "Invalid number of elements epoch" << std::endl;
        // return -1;
    }
    // std::cout << "(1)" << std::endl;
    float predictions = predict(vectors);
    // std::cout << "(2)" << std::endl;
    matrix *thetas = gradient(vectors, predictions, y);
    // std::cout << "(3)" << std::endl;
    //update weights
    // std::cout << "\nrecieved thetas:" << std::endl;
    // thetas.print();
    // std::cout << "\nweights before:" << std::endl;
    // b.print();
    // w.print();
    int cols = thetas->getColumns();
    // std::cout << "(4)" << std::endl;
    for (int j = 0; j < cols - 1; j++) {
        w.table[0][j] -= learningRate * thetas->table[0][j];
    }
    // std::cout << "(5)" << std::endl;
    b.table[0][0] -= learningRate * thetas->table[0][cols-1];
    // std::cout << "(6)" << std::endl;
    // std::cout << "\nweights after:" << std::endl;
    // b.print();
    // w.print();

    delete thetas;
    return this->abs(predict(vectors) - y);
}