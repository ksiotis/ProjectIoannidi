#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "scheduler.hpp"

class matrix {
private:
    int size[2];
public:
    float **table;

    matrix(int row, int column);
    // matrix(const matrix &mat);
    ~matrix();

    int getRows();
    int getColumns();

    // static float sum();
    static matrix *dot(matrix &a, matrix &b);
    static matrix *dot(matrix &a, matrix &b, scheduler &sch);
    // static matrix multiplication();
    matrix *row(int row);
    matrix *shuffleRows(int *y, int rows, int* &rowy);
    matrix *shuffleRows(int *y, int rows, int* &rowy, scheduler &sch);

    void print();
};

#endif /*MATRIX_HPP*/