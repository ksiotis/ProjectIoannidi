#include <iostream>
// #include <cmath>
#include <fstream>
#include <cmath>
#include <cstring>
#include "matrix.hpp"
#include "logistic_regression.hpp"

float logistic_regression::abs(float v) {
    return v < 0 ? -v : v;
}

float logistic_regression::compare(matrix &values, int *actual_values) {
    float L = 0;
    for (int i = 0, rows = values.getRows(); i < rows; i++) {
        if (actual_values[i])
            L += log10(values.table[i][0] - actual_values[i]);
        else
            L += log10(1 - values.table[i][0]);
    }

    return L;
}

void compareThreads(void *arg) {

    //get arguments
    void **argv = (void **)arg;
    conditionVariable *cond = (conditionVariable *)argv[0];
    int *doneCounter = (int *)argv[1];
    int start = *(int *)argv[2];
    int end = *(int *)argv[3];
    float *L = (float *)argv[4];
    semaphore *mutexL = (semaphore *)argv[5];
    int *actual_values = (int *)argv[6];
    float **values = (float **)argv[7];

    //main function
    float temp = 0;
    for (int i = start; i < end; i++) {
        if (actual_values[i])
            temp += log10(values[i][0] - actual_values[i]);
        else
            temp += log10(1 - values[i][0]);
    }
    
    mutexL->lock();
    *L += temp;
    mutexL->unlock();

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

float logistic_regression::compare(matrix &values, int *actual_values, scheduler &sch) {
    float L = 0;
    semaphore mutexL;
    int rows = values.getRows();

    //prepair argument to pass
    conditionVariable doneMutex;
    int doneCounter = 0;

    int div = rows / sch.execution_threads;
    int mod = rows % sch.execution_threads;

    int end; 
    for (int start = 0; start < rows; start = end) {
        if (start < (div + 1) * mod)
            end = start + div + 1;
        else
            end = start + div;


        void **args = new void *[8];
        args[0] = &doneMutex;
        args[1] = &doneCounter;
        args[2] = new int(start); //delete inside
        args[3] = new int(end); //delete inside
        args[4] = &L;
        args[5] = &mutexL;
        args[6] = actual_values;
        args[7] = values.table;

        doneMutex.lock(); //pass task to scheduler
        sch.addTask(new task(compareThreads, (void *)args));
        doneCounter++;
        doneMutex.unlock();
    }

    //wait for all tasks to finish
    doneMutex.lock();
    while (doneCounter > 0) {
        doneMutex.wait();
    }
    doneMutex.unlock();

    return L;
}

float logistic_regression::accuracy(matrix &values, int *actual_values) {
    //assumes actual_values has correct lenght*/
    int count = 0;
    try {
        for (int i = 0, rows = values.getRows(); i < rows; i++) {
            if ((values.table[i][0] < 0.5 ? 0 : 1) == actual_values[i]) {
                count++;
            }
        }
        return (float)(count) / values.getRows() * 100;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return -2;
    }
}

// void accuracyThreads(void *arg) {
//     //get arguments
//     void **argv = (void **)arg;
//     conditionVariable *cond = (conditionVariable *)argv[0];
//     int *doneCounter = (int *)argv[1];
//     int start = *(int *)argv[2];
//     int end = *(int *)argv[3];
//     int *count = (int *)argv[4];
//     semaphore *mutexCount = (semaphore *)argv[5];
//     int *actual_values = (int *)argv[6];
//     float **values = (float **)argv[7];

//     //main function
//     int temp = 0;
//     for (int i = start; i < end; i++) {
//         if ((values[i][0] < 0.5 ? 0 : 1) == actual_values[i]) {
//             temp++;
//         }
//     }
    
//     mutexCount->lock();
//     *count += temp;
//     mutexCount->unlock();

//     cond->lock();
//     if (--(*doneCounter) != 0) {
//         cond->unlock();
//     }
//     else {
//         cond->unlock();
//         cond->signal();
//     }

//     delete (int *)argv[2];
//     delete (int *)argv[3];
//     delete[] argv;
// }

void accuracyThreads(void *arg) {
    //get arguments
    void **argv = (void **)arg;
    conditionVariable *cond = (conditionVariable *)argv[0];
    int *doneCounter = (int *)argv[1];
    int i = *(int *)argv[2];
    // int end = *(int *)argv[3];
    int *count = (int *)argv[3];
    semaphore *mutexCount = (semaphore *)argv[4];
    int *actual_values = (int *)argv[5];
    float **values = (float **)argv[6];

    //main function
    int temp = 0;
    if ((values[i][0] < 0.5 ? 0 : 1) == actual_values[i]) {
        temp++;
    }
    
    mutexCount->lock();
    *count += temp;
    mutexCount->unlock();

    cond->lock();
    if (--(*doneCounter) != 0) {
        cond->unlock();
    }
    else {
        cond->unlock();
        cond->signal();
    }

    delete (int *)argv[2];
    // delete (int *)argv[3];
    delete[] argv;
}

float logistic_regression::accuracy(matrix &values, int *actual_values, scheduler &sch) {
    //assumes actual_values has correct lenght*/
    int count = 0;
    semaphore countMutex;
    
    //prepair argument to pass
    conditionVariable doneMutex;
    int doneCounter = 0;
    int rows = values.getRows();

    // int div = rows / sch.execution_threads;
    // int mod = rows % sch.execution_threads;

    // int end; 
    for (int i = 0; i < rows; i++) {
    // for (int start = 0; start < rows; start = end) {
    //     if (start < (div + 1) * mod)
    //         end = start + div + 1;
    //     else
    //         end = start + div;

        void **args = new void *[7];
        args[0] = &doneMutex;
        args[1] = &doneCounter;
        args[2] = new int(i); //delete inside
        // args[3] = new int(end); //delete inside
        args[3] = &count;
        args[4] = &countMutex;
        args[5] = actual_values;
        args[6] = values.table;

        doneMutex.lock(); //pass task to scheduler
        sch.addTask(new task(accuracyThreads, (void *)args));
        doneCounter++;
        doneMutex.unlock();
    }

    //wait for all tasks to finish
    doneMutex.lock();
    while (doneCounter > 0) {
        doneMutex.wait();
    }
    doneMutex.unlock();

    return (float)(count) / rows * 100;
}

void logistic_regression::sigmoid(matrix &x) {
    /*aproximate sigmoid function, faster but less accurate*/
    int rows = x.getRows();
    int columns = x.getColumns();
    // if (columns != 1) {
    //     std::cerr << "Provided wrong table sigmoid";
    // }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            x.table[i][j] = (0.5f * x.table[i][j]) / (1 + logistic_regression::abs(x.table[i][j])) + 0.5f;
        }
    }
}

void sigmoidThread(void *arg) {
    //get arguments
    void **argv = (void **)arg;
    conditionVariable *cond = (conditionVariable *)argv[0];
    int *doneCounter = (int *)argv[1];
    int j = *(int *)argv[2];
    int rows = *(int *)argv[3];
    float **table = (float **)argv[4];

    //main function
    for (int i = 0; i < rows; i++) {
        table[i][j] = (0.5f * table[i][j]) / (1 + logistic_regression::abs(table[i][j])) + 0.5f;
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

void logistic_regression::sigmoid(matrix &x, scheduler &sch) {
    /*aproximate sigmoid function, faster but less accurate*/
    int rows = x.getRows();
    int columns = x.getColumns();
    // if (columns != 1) {
    //     std::cerr << "Provided wrong table sigmoid";
    // }

    //prepair argument to pass
    conditionVariable doneMutex;
    int doneCounter = 0;
    
    for (int j = 0; j < columns; j++) {

        void **args = new void *[5];
        args[0] = &doneMutex;
        args[1] = &doneCounter;
        args[2] = new int(j); //delete inside
        args[3] = new int(rows); //delete inside
        args[4] = x.table;

        doneMutex.lock(); //pass task to scheduler
        sch.addTask(new task(sigmoidThread, (void *)args));
        doneCounter++;
        doneMutex.unlock();
    }

    //wait for all tasks to finish
    doneMutex.lock();
    while (doneCounter > 0) {
        doneMutex.wait();
    }
    doneMutex.unlock();
}

void logistic_regression::sigmoid(matrix *x) {
    logistic_regression::sigmoid(*x);
}

void logistic_regression::sigmoid(matrix *x, scheduler &sch) {
    logistic_regression::sigmoid(*x, sch);
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
    }
    predictions = NULL;
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

    matrix *thetas = new matrix(rows, columns); //extra position for b

    for (int i = 0; i < rows; i++) {
        float error = predictions.table[i][0] - y[i];
        for (int j = 0; j < columns; j++) {
            thetas->table[0][j] += error * vectors.table[i][j];
        }
    }
    return thetas;
}

void gradientFillErrorThreads(void *arg) {
    //get arguments
    void **argv = (void **)arg;
    conditionVariable *cond = (conditionVariable *)argv[0];
    int *doneCounter = (int *)argv[1];
    int i = *(int *)argv[2];
    int columns = *(int *)argv[3];
    int *y = (int *)argv[4];
    float **predictions = (float **)argv[5];
    float *error = (float *)argv[6];

    //main function
    for (int j = 0; j < columns; j++) {
        error[i] = predictions[i][0] - y[i];
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

void updateThetasThreads(void *arg) {
    //get arguments
    void **argv = (void **)arg;
    conditionVariable *cond = (conditionVariable *)argv[0];
    int *doneCounter = (int *)argv[1];
    int rows = *(int *)argv[2];
    int start = *(int *)argv[3];
    int end = *(int *)argv[4];
    float **thetas = (float **)argv[5];
    float *error = (float *)argv[6];
    float **vectors = (float **)argv[7];

    //main function
    for (int j = start; j < end; j++) {
        float temp = 0;
        for (int i = 0; i < rows; i++) {
            temp += error[i] * vectors[i][j];
        }
        thetas[0][j] += temp;
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
    delete[] argv;
}

matrix *logistic_regression::gradient(matrix &vectors, matrix &predictions, int *y, scheduler &sch) {
    if (vectors.getColumns() != w.getColumns()) {
        std::cerr << "Invalid number of elements" << std::endl;
        // return;
    }
    int rows = vectors.getRows();
    int columns = vectors.getColumns();

    matrix *thetas = new matrix(rows, columns);


    float *error = new float[rows];

    // //~~~~~fill errors~~~~~~
    // //prepair argument to pass
    conditionVariable doneMutex;
    int doneCounter = 0;

    for (int i = 0; i < rows; i++) {

        void **args = new void *[7];
        args[0] = &doneMutex;
        args[1] = &doneCounter;
        args[2] = new int(i); //delete inside
        args[3] = new int(columns); //delete inside
        args[4] = y;
        args[5] = predictions.table;
        args[6] = error;

        doneMutex.lock(); //pass task to scheduler
        sch.addTask(new task(gradientFillErrorThreads, (void *)args)); //bad for stohastic, good for minibatch
        doneCounter++;
        doneMutex.unlock();
    }

    //wait for all tasks to finish
    doneMutex.lock();
    while (doneCounter > 0) {
        doneMutex.wait();
    }
    doneMutex.unlock();

    //~~~~~update thetas~~~~~~
    //prepair argument to pass
    // conditionVariable doneMutex;
    // int doneCounter = 0;

    int div = columns / sch.execution_threads;
    int mod = columns % sch.execution_threads;

    int end;
    for (int start = 0; start < rows; start = end) {
        if (start < (div + 1) * mod)
            end = start + div + 1;
        else
            end = start + div;

        void **args = new void *[8];
        args[0] = &doneMutex;
        args[1] = &doneCounter;
        args[2] = new int(rows); //delete inside
        args[3] = new int(start); //delete inside
        args[4] = new int(end); //delete inside
        args[5] = thetas->table;
        args[6] = error;
        args[7] = vectors.table;

        doneMutex.lock(); //pass task to scheduler
        sch.addTask(new task(updateThetasThreads, (void *)args));
        doneCounter++;
        doneMutex.unlock();
    }

    //wait for all tasks to finish
    doneMutex.lock();
    while (doneCounter > 0) {
        doneMutex.wait();
    }
    doneMutex.unlock();

    delete[] error;

    return thetas;
}

matrix *logistic_regression::predict(matrix &vectors) {
    if (vectors.getColumns() != w.getColumns()) {
        std::cerr << "Invalid number of elements predict" << std::endl;
        // return -1;
    }

    matrix *temp = matrix::dot(vectors, w);
    sigmoid(temp);
    return temp;
}

matrix *logistic_regression::predict(matrix &vectors, scheduler &sch) {
    if (vectors.getColumns() != w.getColumns()) {
        std::cerr << "Invalid number of elements predict" << std::endl;
        // return -1;
    }

    matrix *temp = matrix::dot(vectors, w, sch);
    sigmoid(temp, sch);
    return temp;
}

void logistic_regression::epoch(matrix &vectors, int *y) {
    if (vectors.getColumns() != w.getColumns()) {
        std::cerr << "Invalid number of elements epoch" << std::endl;
        // return -1;
    }

    if (predictions != NULL)
        delete predictions;
    predictions = predict(vectors);
    if (predictions->getRows() != vectors.getRows()) {
        std::cerr << "predictions mismatch" << std::endl;
    }

    matrix *thetas = gradient(vectors, *predictions, y);
    if (thetas->getRows() != vectors.getRows()) {
        std::cerr << "thetas mismatch" << std::endl;
    }
    
    //update weights
    int rows = thetas->getRows();
    int cols = thetas->getColumns();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            w.table[0][j] -= learningRate * thetas->table[i][j];
        }
    }

    delete thetas;
}

void updateWeightsEpochThreads(void *arg) {
    //get arguments
    void **argv = (void **)arg;
    conditionVariable *cond = (conditionVariable *)argv[0];
    int *doneCounter = (int *)argv[1];
    int start = *(int *)argv[2];
    int end = *(int *)argv[3];
    int rows = *(int *)argv[4];
    float learningRate = *(float *)argv[5];
    float **thetas = (float **)argv[6];
    float **w = (float **)argv[7];

    //main function
    float temp = 0;
    for (int j = start; j < end; j++) {
        for (int i = 0; i < rows; i++) {
            temp += learningRate * thetas[i][j];
        }
        w[0][j] -= temp;
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
    delete[] argv;
}

void logistic_regression::epoch(matrix &vectors, int *y, scheduler &sch) {

    if (vectors.getColumns() != w.getColumns()) {
        std::cerr << "Invalid number of elements epoch" << std::endl;
        // return -1;
    }
    if (predictions != NULL)
        delete predictions;
    predictions = predict(vectors, sch);

    matrix *thetas = gradient(vectors, *predictions, y, sch);
    
    //update weights
    int rows = thetas->getRows();
    int cols = thetas->getColumns();

    //prepair argument to pass
    conditionVariable doneMutex;
    int doneCounter = 0;
    int div = cols / sch.execution_threads;
    int mod = cols % sch.execution_threads;

    int end;
    for (int start = 0; start < rows; start = end) {
        if (start < (div + 1) * mod)
            end = start + div + 1;
        else
            end = start + div;


        void **args = new void *[8];
        args[0] = &doneMutex;
        args[1] = &doneCounter;
        args[2] = new int(start); //delete inside
        args[3] = new int(end); //delete inside
        args[4] = new int(rows); //delete inside
        args[5] = &learningRate;
        args[6] = thetas->table;
        args[7] = w.table;

        doneMutex.lock(); //pass task to scheduler
        sch.addTask(new task(updateWeightsEpochThreads, (void *)args));
        doneCounter++;
        doneMutex.unlock();
    }

    //wait for all tasks to finish
    doneMutex.lock();
    while (doneCounter > 0) {
        doneMutex.wait();
    }
    doneMutex.unlock();

    delete thetas;
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