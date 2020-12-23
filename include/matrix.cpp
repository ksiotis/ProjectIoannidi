#include <iostream>
#include <cstring>
#include <iomanip>
#include "matrix.hpp"

matrix::matrix(int row, int column) {
    size[0] = row;
    size[1] = column;
    try {
        table = new float *[row];
        for (int i = 0; i < row; i++) {
            table[i] = new float[column];
            for (int j = 0; j < column; j++)
                table[i][j] = 0;
            // memset(table[i], 0, sizeof(float) * column);
        }
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        exit(-1);
    }
}

// matrix::matrix(const matrix &mat) {
//     size[0] = mat.size[0];
//     size[1] = mat.size[1];
//     table = new float *[size[0]];
//     for (int i = 0; i < size[0]; i++) {
//         table[i] = new float[size[1]];
//         for (int j = 0; j < size[1]; j++) {
//             table[i][j] = mat.table[i][j];

//             // std::cout << mat.table[i][j] << " - " << table[i][j] << std::endl;
//         }
//     }
// }

matrix::~matrix() {
    for (int i = 0; i < size[0]; i++) {
        delete[] table[i];
        // if (memcmp(table[i], 0, )  != NULL) {
        // }
    }
    delete[] table;
}

int matrix::getRows() { return size[0]; }

int matrix::getColumns() { return size[1]; }

float matrix::dot(matrix &a, matrix &b) {
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

matrix *matrix::row(int row) {
    /*row starts from 0*/
    if (row >= size[0]) {
        std::cerr << "No such row in matrix";
        return NULL;
    }

    matrix *ret = new matrix(1, size[1]);
    for (int i = 0; i < size[1]; i++) {
        ret->table[0][i] = table[row][i];
    }
    return ret;
}

void matrix::print() {
    for (int i = 0; i < size[0]; i++) {
        std::cout << "row " << i << ":" << std::endl;
        for (int j = 0; j < size[1]; j++) {
            std::cout << std::fixed << std::setprecision(7) << table[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}