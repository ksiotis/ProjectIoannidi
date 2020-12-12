#ifndef TF_IDF
#define TF_IDF

#include <iostream>
#include <string>
#include <fstream>

#include "list.hpp"

class clique;

class Index {
private:
    std::string id;
    int nt; // nt as presented in Project's pdf file
    int dim; // dimension
    float idf; 
public:
    Index(std::string a,int dimension);
    ~Index();
    
    std::string getWord();

    void raiseNt();
    int getNt();

    void setDim(int a);
    int getDim();

    void setIdf(float a);
    float getIdf();

    void print();
};

#endif /* SPEC_HPP */