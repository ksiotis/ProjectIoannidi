#ifndef MATRIX_HPP
#define MATRIX_HPP

class matrix {
private:
    int size[2];
public:
    float **table;

    matrix(int row, int column);
    matrix(const matrix &mat);
    ~matrix();

    int getRows();
    int getColumns();

    static float sum();
    static float dot(matrix a, matrix b);
    // static matrix multiplication();

    void print();
};

#endif /*MATRIX_HPP*/