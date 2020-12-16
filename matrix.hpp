#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>

class matrix {
private:
    int size[2];
public:
    float **table = NULL;

    matrix(int row, int column);
    ~matrix();

    // void set(int row, int column, float value);
    // float get(int row, int column);

    matrix multiplication();
    matrix operator*(matrix &m2);
};

matrix::matrix(int row, int column) {
    size[0] = row;
    size[1] = column;
    table = new float *[row];
    for (int i = 0; i < row; i++) {
        table[i] = new float[column];
    }
}

matrix::~matrix() {
    for (int i = 0; i < size[0]; i++) {
        delete[] table[i];
    }
    delete[] table;
}

// void matrix::set(int row, int column, float value) {
//     table[row][column] = value;
// }

// float matrix::get(int row, int column) {
//     return table[row][column];
// }



#endif /*MATRIX_HPP*/