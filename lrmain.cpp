#include <iostream>
#include <sstream>
#include "include/matrix.hpp"
#include "include/logistic_regression.hpp"
#include "include/tf_idf.hpp"

matrix *absoluteDifference(matrix a, matrix b) {
    if (a.getRows() != b.getRows() || a.getColumns() != b.getColumns()) {
        std::cerr << "Invalid number of elements absoluteDifference" << std::endl;
        return NULL;
    }
    int rows = a.getRows();
    int columns = a.getColumns();

    matrix ret(rows, columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            ret.table[i][j] = a.table[i][j] > b.table[i][j] ? a.table[i][j] - b.table[i][j] : b.table[i][j] - a.table[i][j];
        }
    }
    
    ret.table[0][0] = a;
    ret.table[0][1] = b;
    ret.table[0][2] = c;

    return ret;
}

int main() {
    logistic_regression lr(10.0f, 1, 3);




    std::stringstream ss;
    std::string buf;
    Index index = Index(10);

    
    json_index j1 = json_index("first",10);
    std::string testing_heap = "LOL OMG HAHA";
    ss.str(testing_heap);
    while (ss >> buf){
        insert_word(&index,&j1,buf);
    }
    ss.clear();
    j1.fix_Tf();

    json_index j2 = json_index("second",10);
    testing_heap = "LOL OMG HAHA";
    ss.str(testing_heap);
    while (ss >> buf){
        insert_word(&index,&j2,buf);
    }
    ss.clear();
    j2.fix_Tf();
    
    json_index j3 = json_index("third",10);
    testing_heap = "LOL OMG HAHA";
    ss.str(testing_heap);
    while (ss >> buf){
        insert_word(&index,&j3,buf);
    }
    ss.clear();
    j3.fix_Tf();


    index.fix_idf(3); // num of json_files
    index.fix_dim();


    int vec_size = index.get_words_counter();
    float vec[vec_size];
    for(int i=0;i<vec_size;i++){
        vec[i] = 0;
    }
    matrix ma = matrix(3,vec_size);





    get_vector_tfidf(&index,&j1,vec);
    ma.table[0] = vec;
    for(int i=0;i<vec_size;i++){
        vec[i] = 0;
    }
    get_vector_tfidf(&index,&j2,vec);
    ma.table[1] = vec;
    for(int i=0;i<vec_size;i++){
        vec[i] = 0;
    }
    get_vector_tfidf(&index,&j3,vec);
    ma.table[2] = vec;
    for(int i=0;i<vec_size;i++){
        vec[i] = 0;
    }
    
    std::cout << lr.epoch(temp, 1) << std::endl;



    matrix temp = vector(1, 2, 3);
    std::cout << lr.epoch(temp, 1) << std::endl;
    // delete temp;
    temp = vector(10, 20, 30);
    std::cout << lr.epoch(temp, 0) << std::endl;
    // lr.epoch(temp, 0);
    // delete temp;
    temp = vector(0, 1, 0);
    std::cout << lr.epoch(temp, 1) << std::endl;
    // lr.epoch(temp, 1);
    // delete temp;
    temp = vector(100, 2, 10);
    std::cout << lr.epoch(temp, 0) << std::endl;
    // delete temp;
    // lr.epoch(temp, 0);

    std::cout << (float)(lr.predict(vector(100,30,90))) << std::endl;
}