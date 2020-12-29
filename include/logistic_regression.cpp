#include <iostream>
// #include <cmath>
#include <fstream>
#include "matrix.hpp"
#include "logistic_regression.hpp"

float logistic_regression::abs(float v) {
    return v < 0 ? -v : v;
}

float logistic_regression::compare(matrix &values, int *actual_values) {
    /*returns sum of absolute difference,
    assumes actual_values has correct lenght*/
    try {
        float sum = 0;
        for (int i = 0, rows = values.getRows(); i < rows; i++) {
            sum += abs(values.table[i][0] - actual_values[i]);
        }
        return sum;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return -2;
    }
}

void logistic_regression::sigmoid(matrix &x) {
    /*aproximate sigmoid function, faster but less accurate*/
    int rows = x.getRows();
    int columns = x.getColumns();
    if (columns != 1) {
        std::cerr << "Provided wrong table sigmoid";
    }
    
    for (int i = 0; i < rows; i++) {
        x.table[i][0] = 0.5f * x.table[i][0] / (1 + abs(x.table[i][0])) + 0.5f;
    }
}

void logistic_regression::sigmoid(matrix *x) {
    logistic_regression::sigmoid(*x);
}

// float logistic_regression::sigmoid(float x) {
//     /*aproximate sigmoid function, faster but less accurate*/
//     return 1 / exp(-x);
// }

logistic_regression::logistic_regression(float learningRate, int columns, bool random):
    learningRate(learningRate),
    w(1, columns),
    b(1, 1)
{
    if (random) {
        for (int j = 0; j < columns; j++) {
            w.table[0][j] = (float)(rand())/RAND_MAX;
        }
        b.table[0][0] = (float)(rand())/RAND_MAX;

        predictions = NULL;
    }
}

logistic_regression::~logistic_regression() {
    if (predictions != NULL)
        delete predictions;
}

matrix *logistic_regression::getWeights() { return &w; }

matrix *logistic_regression::getBias() { return &b; }

matrix *logistic_regression::gradient(matrix &vectors, matrix &predictions, int *y) {
    if (vectors.getColumns() != w.getColumns()) {
        std::cerr << "Invalid number of elements" << std::endl;
        // return;
    }
    int rows = vectors.getRows();
    int columns = vectors.getColumns();

    // std::cout << rows << "  " << columns + 1 << std::endl;
    matrix *thetas = new matrix(rows, columns + 1); //extra position for b

    for (int i = 0; i < rows; i++) {
        float error = predictions.table[i][0] - y[i];
        for (int j = 0; j < columns; j++) {
            thetas->table[0][j] += error * vectors.table[i][j];
        }
        thetas->table[i][columns] += error;
    }
    return thetas;
}

matrix *logistic_regression::predict(matrix &vectors) {
    if (vectors.getColumns() != w.getColumns()) {
        std::cerr << "Invalid number of elements predict" << std::endl;
        // return -1;
    }

    matrix *temp = matrix::dot(vectors, w);
    int rows = vectors.getRows();
    for (int i = 0; i < rows; i++) {
        temp->table[i][0] += b.table[0][0];
    }
    sigmoid(temp);
    return temp;
}

float logistic_regression::epoch(matrix &vectors, int *y) {
    if (vectors.getColumns() != w.getColumns()) {
        std::cerr << "Invalid number of elements epoch" << std::endl;
        // return -1;
    }

    if (predictions != NULL)
        delete predictions;
    predictions = predict(vectors);

    matrix *thetas = gradient(vectors, *predictions, y);
    
    //update weights
    int rows = thetas->getRows();
    int cols = thetas->getColumns();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols - 1; j++) {
            w.table[0][j] -= learningRate * thetas->table[i][j];
        }
        b.table[0][0] -= learningRate * thetas->table[i][cols-1];
    }

    predictions = predict(vectors);

    float error = cost(y);

    delete thetas;
    return error;
}

float logistic_regression::cost(int *y) {
    int rows = predictions->getRows();
    bool overflow = false; //flag to check if error overflows(could happen for too many rows)
    float error = 0;
    for (int i = 0, rows = predictions->getRows(); i < rows; i++) {
        error += abs(predictions->table[i][0] - y[i]);//n+1
        if (error < 0) {
            overflow = true;
            break;
        }
    }
    if (overflow) {
        for (int i = 0, rows = predictions->getRows(); i < rows; i++) {
            error += abs(predictions->table[i][0] - y[i]) / rows;//2n
        }
    }
    else
        error /= rows;

    return error;
}

logistic_regression *logistic_regression::loadModel(const char *filename) {
    std::ifstream ifile;
    ifile.open(filename);
    if (!ifile) {
        std::cerr << "Cant't open new file";
        return NULL;
    }
    float learningRate;
    float columns;
    ifile >> learningRate;
    ifile >> columns;
    
    logistic_regression *returnable = new logistic_regression(learningRate, columns, false);
    float **mytable = returnable->getWeights()->table;
    for (int i = 0; i < columns; i++) {
        ifile >> mytable[0][i];
    }
    ifile >> returnable->getBias()->table[0][0];

    return returnable;
}

void logistic_regression::extractModel(const char *filename) {

    //check if output file already exists
    std::ifstream outfile;
    outfile.open(filename);
    if (outfile) {
        std::cerr << "File already exists!";
        return;
    }

    //create output file
    std::ofstream ofile;
    ofile.open(filename);
    if (!ofile) {
        std::cerr << "Cant't open new file";
        return;
    }

    //write everything from logistic regression
    int columns = w.getColumns();
    ofile << learningRate << ' ' << columns << ' ';
    for (int i = 0; i < columns; i++) {
        ofile << w.table[0][i] << ' ';
    }
    ofile << b.table[0][0] << ' ';
    ofile.close();
}