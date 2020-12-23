#include <iostream>
#include <sstream>
#include "include/matrix.hpp"
#include "include/logistic_regression.hpp"
#include "include/tf_idf.hpp"

matrix *absoluteDifference(matrix &a, matrix &b) {
    if (a.getRows() != b.getRows() || a.getColumns() != b.getColumns()) {
        std::cerr << "Invalid number of elements absoluteDifference" << std::endl;
        return NULL;
    }
    int rows = a.getRows();
    int columns = a.getColumns();

    matrix *ret = new matrix(rows, columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            ret->table[i][j] = a.table[i][j] > b.table[i][j] ? a.table[i][j] - b.table[i][j] : b.table[i][j] - a.table[i][j];
        }
    }

    return ret;
}

int main() {




    std::stringstream ss;
    std::string buf;
    Index index = Index(10);

    
    json_index j1 = json_index("first",1);
    std::string testing_heap = "The cat (Felis catus) is a domestic species of small carnivorous mammal.[1][2] It is the only domesticated species in the family Felidae and is often referred to as the domestic cat to distinguish it from the wild members of the family.[4] A cat can either be a house cat, a farm cat or a feral cat; the latter ranges freely and avoids human contact.[5] Domestic cats are valued by humans for companionship and their ability to hunt rodents. About 60 cat breeds are recognized by various cat registries.";
    ss.str(testing_heap);
    while (ss >> buf){
        insert_word(&index,&j1,buf);
    }
    ss.clear();
    j1.fix_Tf();

    json_index j2 = json_index("second",1);
    testing_heap = "The domestic cat is a complex creature and unfortunately, problems can arise for cats because sometimes we do not understand their natural drives and reactions";
    ss.str(testing_heap);
    while (ss >> buf){
        insert_word(&index,&j2,buf);
    }
    ss.clear();
    j2.fix_Tf();
    
    json_index j3 = json_index("third",1);
    testing_heap = "The dog (Canis familiaris when considered a distinct species or Canis lupus familiaris when considered a subspecies of the wolf)[5] is a domesticated carnivore of the family Canidae. It is part of the wolf-like canids,[6] and is the most widely abundant terrestrial carnivore.[7][8][9][10][11] The dog and the extant gray wolf are sister taxa as modern wolves are not closely related to the wolves that were first domesticated,[12][13][14] which implies that the direct ancestor of the dog is extinct.[15] The dog was the first species to be domesticated,[14][16] and has been selectively bred over millennia for various behaviors, sensory capabilities, and physical attributes.";
    ss.str(testing_heap);
    while (ss >> buf){
        insert_word(&index,&j3,buf);
    }
    ss.clear();
    j3.fix_Tf();


    index.fix_idf(3); // num of json_files
    index.fix_dim();


    int vec_size = index.get_words_counter();
    // float vec[vec_size];
    // for(int i=0;i<vec_size;i++){
    //     vec[i] = 0;
    // }
    logistic_regression lr(10.0f, 1, vec_size);
    matrix ma(3, vec_size);


    get_vector_tfidf(&index,&j1,ma.table[0]);
    // ma.table[0] = vec;
    // for(int i=0;i<vec_size;i++){
    //     vec[i] = 0;
    // }
    get_vector_tfidf(&index,&j2,ma.table[1]);
    // ma.table[0] = vec;
    // for(int i=0;i<vec_size;i++){
    //     vec[i] = 0;
    // }
    get_vector_tfidf(&index,&j3,ma.table[2]);
    // ma.table[0] = vec;
    // for(int i=0;i<vec_size;i++){
    //     vec[i] = 0;
    // }
    
    matrix **matrices = new matrix *[9];
    for (int i = 0; i < ma.getRows(); i++) {
        for (int j = 0; j < ma.getRows(); j++) {
            matrix *row1 = ma.row(i), *row2 = ma.row(j);
            matrices[i*3+j] = absoluteDifference(*row1,*row2);
            delete row1;
            delete row2;
        }
    }

    for (int i = 0; i < 9; i++) {
        std::cout << lr.epoch(*matrices[i], (i == 2 || i == 5 || i == 6 || i == 7) ? 0 : 1) << std::endl;
    }

    for (int i = 0; i < 9; i++) {
        delete matrices[i];
    }
    delete[] matrices;

    // matrix temp = vector(1, 2, 3);
    // std::cout << lr.epoch(temp, 1) << std::endl;
    // // delete temp;
    // temp = vector(10, 20, 30);
    // std::cout << lr.epoch(temp, 0) << std::endl;
    // // lr.epoch(temp, 0);
    // // delete temp;
    // temp = vector(0, 1, 0);
    // std::cout << lr.epoch(temp, 1) << std::endl;
    // // lr.epoch(temp, 1);
    // // delete temp;
    // temp = vector(100, 2, 10);
    // std::cout << lr.epoch(temp, 0) << std::endl;
    // // delete temp;
    // // lr.epoch(temp, 0);

    // std::cout << (float)(lr.predict(vector(100,30,90))) << std::endl;
}