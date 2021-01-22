#include <iostream>
#include <cstring>
#include <iomanip>
#include "matrix.hpp"
#include "thread.hpp"
#include "scheduler.hpp"

matrix::matrix(int row, int column) {
    size[0] = row;
    size[1] = column;
    // std::cout << row << "  " << column << std::endl;
    table = new float *[row];
    for (int i = 0; i < row; i++) {
        table[i] = new float[column];
        // for (int j = 0; j < column; j++)
        //     table[i][j] = 0.0f;
        memset(table[i], 0, sizeof(float) * column);
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
    }
    delete[] table;
}

int matrix::getRows() { return size[0]; }

int matrix::getColumns() { return size[1]; }

matrix *matrix::dot(matrix &a, matrix &b) {
    //b is (1,n)
    if (a.getColumns() != b.getColumns()) {
        std::cerr << "Invalid element number dot" << std::endl;
        // return -1;
    }
    if (b.getRows() != 1)
        std::cerr << "second table of dot has many rows, using only the first" << std::endl;

    int rows = a.getRows();
    int columns = a.getColumns();
    matrix *returnable = new matrix(rows, 1);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            returnable->table[i][0] += a.table[i][j] * b.table[0][j];
        }
    }

    return returnable;
}

void dotThread(void *arg) {
    //get arguments
    void **argv = (void **)arg;
    conditionVariable *cond = (conditionVariable *)argv[0];
    int *doneCounter = (int *)argv[1];
    int i = *(int *)argv[2];
    int columns = *(int *)argv[3];
    float **returntable = (float **)argv[4];
    float **atable = (float **)argv[5];
    float **btable = (float **)argv[6];

    //main function
    for (int j = 0; j < columns; j++) {
        returntable[i][0] += atable[i][j] * btable[0][j];
    }

    cond->lock();
    if (--(*doneCounter) != 0) {
        cond->unlock();
    }
    else {
        cond->unlock();
        cond->signal();
    }

    delete (int *)argv[2];
    delete (int *)argv[3];
    delete[] argv;
}

matrix *matrix::dot(matrix &a, matrix &b, scheduler &sch) {
    //b is (1,n)
    if (a.getColumns() != b.getColumns()) {
        std::cerr << "Invalid element number dot" << std::endl;
        // return -1;
    }
    if (b.getRows() != 1)
        std::cerr << "second table of dot has many rows, using only the first" << std::endl;

    int rows = a.getRows();
    int columns = a.getColumns();
    matrix *returnable = new matrix(rows, 1);
    
    //prepair argument to pass
    conditionVariable doneMutex;
    int doneCounter = 0;

    for (int i = 0; i < rows; i++) {

        void **args = new void *[7];
        args[0] = &doneMutex;
        args[1] = &doneCounter;
        args[2] = new int(i); //delete inside
        args[3] = new int(columns); //delete inside
        args[4] = returnable->table;
        args[5] = a.table;
        args[6] = b.table;

        doneMutex.lock(); //pass task to scheduler
        sch.addTask(new task(dotThread, (void *)args));
        doneCounter++;
        doneMutex.unlock();
    }

    //wait for all tasks to finish
    doneMutex.lock();
    while (doneCounter > 0) {
        doneMutex.wait();
    }
    doneMutex.unlock();

    return returnable;
}

matrix *matrix::rows(int start, int end) {
    /*row starts from 0*/
    int rows = size[0];
    int columns = size[1];
    if (start >= rows) {
        std::cerr << "No such row in matrix";
        return NULL;
    }
    if (end > rows)
        end = rows;

    matrix *ret = new matrix(end - start, columns);
    for (int j = 0; j < columns; j++) {
        for (int i = start; i < end; i++) {
            ret->table[i-start][j] = table[i][j];
        }
    }
    return ret;
}

void matrixRowThreads(void *arg) {
    //get arguments
    void **argv = (void **)arg;
    conditionVariable *cond = (conditionVariable *)argv[0];
    int *doneCounter = (int *)argv[1];
    int startj = *(int *)argv[2];
    int endj = *(int *)argv[3];
    int starti = *(int *)argv[4];
    int endi = *(int *)argv[5];
    float **source = (float **)argv[6];
    float **destination = (float **)argv[7];

    //main function
    for (int i = starti; i < endi; i++) {
        for (int j = startj; j < endj; j++) {
            destination[i - starti][j] = source[i][j];
        }
    }

    cond->lock();
    if (--(*doneCounter) != 0) {
        cond->unlock();
    }
    else {
        cond->unlock();
        cond->signal();
    }

    delete (int *)argv[2];
    delete (int *)argv[3];
    delete (int *)argv[4];
    delete (int *)argv[5];
    delete[] argv;
}

matrix *matrix::rows(int starti, int endi, scheduler &sch) {
    /*starti starts from 0*/
    int columns = size[1];
    int rows = size[0];
    if (starti >= rows) {
        std::cerr << "No such row in matrix";
        return NULL;
    }
    if (endi > rows)
        endi = rows;

    matrix *ret = new matrix(endi - starti, columns);

    //prepare arguments to pass
    conditionVariable doneMutex;
    int doneCounter = 0;

    int div = columns / sch.execution_threads;
    int mod = columns % sch.execution_threads;

    int endj;
    for (int startj = 0; startj < columns; startj = endj) {
        if (startj < (div + 1) * mod)
            endj = startj + div + 1;
        else
            endj = startj + div;

        void **args = new void *[8];
        args[0] = &doneMutex;
        args[1] = &doneCounter;
        args[2] = new int(startj); //delete inside
        args[3] = new int(endj); //delete inside
        args[4] = new int(starti); //delete inside
        args[5] = new int(endi); //delete inside
        args[6] = this->table;
        args[7] = ret->table;

        doneMutex.lock(); //pass task to scheduler
        sch.addTask(new task(matrixRowThreads, (void *)args));
        doneCounter++;
        doneMutex.unlock();
    }

    //wait for all tasks to finish
    doneMutex.lock();
    while (doneCounter > 0) {
        doneMutex.wait();
    }
    doneMutex.unlock();

    return ret;
}

matrix *matrix::shuffleRows(int *y, int rows, int* &rowy) {
    /*row starts from 0*/
    if (rows >= size[0]) {
        std::cerr << "Too many rows: " << rows << std::endl;
        return NULL;
    }
    if (rows < 1)
        rows = size[0];

    rowy = new int[rows];

    int *shuffledNumbers = new int[size[0]];
    for (int i = 0; i < size[0]; i++) {
        shuffledNumbers[i] = i;
    }
    for (int i = size[0] - 1; i > 0; i--) { //Fisher–Yates shuffling
        int j = rand() % (i+1); 
        int temp = shuffledNumbers[i];
        shuffledNumbers[i] = shuffledNumbers[j]; 
        shuffledNumbers[j] = temp;
    }

    matrix *ret = new matrix(rows, size[1]);

    for (int i = 0; i < rows; i++) {
        int selectedRow = shuffledNumbers[i];
        rowy[i] = y[selectedRow];
        for (int j = 0; j < size[1]; j++) {
            ret->table[i][j] = table[selectedRow][j];
        }
    }
    delete[] shuffledNumbers;
    return ret;
}

void shuffleRowsThreads(void *arg) {
    //get arguments
    void **argv = (void **)arg;
    conditionVariable *cond = (conditionVariable *)argv[0];
    int *doneCounter = (int *)argv[1];
    int i = *(int *)argv[2];
    int columns = *(int *)argv[3];
    int *shuffledNumbers = (int *)argv[4];
    int *rowy = (int *)argv[5];
    int *y = (int *)argv[6];
    float **table = (float **)argv[7];
    float **returntable = (float **)argv[8];

    //main function
    int selectedRow = shuffledNumbers[i];
    rowy[i] = y[selectedRow];
    for (int j = 0; j < columns; j++) {
        returntable[i][j] = table[selectedRow][j];
    }

    cond->lock();
    if (--(*doneCounter) != 0) {
        cond->unlock();
    }
    else {
        cond->unlock();
        cond->signal();
    }

    delete (int *)argv[2];
    delete[] argv;
}

matrix *matrix::shuffleRows(int *y, int rows, int* &rowy, scheduler &sch) {
    /*row starts from 0*/
    if (rows >= size[0]) {
        std::cerr << "Too many rows: " << rows << std::endl;
        return NULL;
    }
    if (rows < 1)
        rows = size[0];

    rowy = new int[rows];

    int *shuffledNumbers = new int[size[0]];
    for (int i = 0; i < size[0]; i++) {
        shuffledNumbers[i] = i;
    }
    for (int i = size[0] - 1; i > 0; i--) { //Fisher–Yates shuffling
        int j = rand() % (i+1); 
        int temp = shuffledNumbers[i];
        shuffledNumbers[i] = shuffledNumbers[j]; 
        shuffledNumbers[j] = temp;
    }

    matrix *ret = new matrix(rows, size[1]);

    //prepair argument to pass
    conditionVariable doneMutex;
    int doneCounter = 0;

    for (int i = 0; i < rows; i++) {

        void **args = new void *[9];
        args[0] = &doneMutex;
        args[1] = &doneCounter;
        args[2] = new int(i); //delete inside
        args[3] = &(size[1]);
        args[4] = shuffledNumbers;
        args[5] = rowy;
        args[6] = y;
        args[7] = table;
        args[8] = ret->table;

        doneMutex.lock(); //pass task to scheduler
        sch.addTask(new task(shuffleRowsThreads, (void *)args));
        doneCounter++;
        doneMutex.unlock();
    }

    //wait for all tasks to finish
    doneMutex.lock();
    while (doneCounter > 0) {
        doneMutex.wait();
    }
    doneMutex.unlock();

    delete[] shuffledNumbers;
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