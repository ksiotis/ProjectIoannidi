#include <iostream>
#include "matrix.hpp"

matrix::matrix(int row, int column) {
    size[0] = row;
    size[1] = column;
    table = new float *[row];
    for (int i = 0; i < row; i++) {
        table[i] = new float[column];
    }
}

matrix::matrix(const matrix &mat) {
    size[0] = mat.size[0];
    size[1] = mat.size[1];
    table = new float *[size[0]];
    for (int i = 0; i < size[0]; i++) {
        table[i] = new float[size[1]];
        for (int j = 0; j < size[1]; j++) {
            table[i][j] = mat.table[i][j];
        }
    }
}

matrix::~matrix() {
    for (int i = 0; i < size[0]; i++) {
        delete[] table[i];
    }
    delete[] table;
}

int matrix::getRows() { return size[0]; }

int matrix::getColumns() { return size[1]; }

float matrix::dot(matrix a, matrix b) {
    if (a.getColumns() != b.getColumns()) {
        std::cerr << "Invalid element number" << std::endl;
        // return -1;
    }
    // matrix returnable(a.getRows(), 1);
    float sum = 0;
    for (int i = 0; i < a.getRows(); i++) {
        for (int j = 0; j < a.getColumns(); j++) {
            sum += a.table[i][j] * b.table[i][j];
        }
    }

    return sum;
}

void matrix::print() {
    for (int i = 0; i < size[0]; i++) {
        for (int j = 0; j < size[1]; j++) {
            std::cout << table[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}