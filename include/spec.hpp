#ifndef SPEC_HPP
#define SPEC_HPP

#include <iostream>
#include <string>
#include <fstream>

#include "list.hpp"
#include "utilities.hpp"

class clique;

class spec: public generic{
private:
    // std::string id;
    clique *myClique;
public:
    spec(std::string a, list<clique> &cliqueContainer);
    ~spec();
    
    // virtual std::string getId();
    clique* getClique();
    void setClique(clique *newClique);

    void merge(spec *second);
    void unsimilar(spec *second);
    void print();
};

class clique {
private:
    list<spec> content;
    list<clique> negative;
public:
    clique(spec* initialSpec);
    ~clique();

    list<spec>* getContentList();
    list<clique>* getNegativeList();

    void merge(clique *second);
    void addNegative(clique *second);
    void writePairs(std::ofstream &ofile);
};

#endif /* SPEC_HPP */