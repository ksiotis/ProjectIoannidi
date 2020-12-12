#include "tf_idf.hpp"

//~~~~~~~spec~~~~~~~~

Index::Index(std::string a, int dimension) {
    id = a;
    nt = 1;
    dim = dimension;
    idf = 0;
}

Index::~Index() {}

std::string Index::getWord() { 
    return id;
}

void Index::raiseNt() {
    nt++;
}

int Index::getNt() {
    return nt;
}

void Index::setDim(int a) {
    dim = a;
}

int Index::getDim() {
    return dim;
}

void Index::setIdf(float a) {
    idf = a;
}

float Index::getIdf() {
    return idf;
}


void Index::print() {
    std::cout << id << std::endl;
}
