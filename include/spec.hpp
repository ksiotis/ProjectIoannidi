#ifndef SPEC_HPP
#define SPEC_HPP

#include <iostream>
#include <string>

#include "list.hpp"

class clique;

class spec {
private:
    std::string id;
    clique *myClique;
public:
    spec(std::string a, list<clique> &cliqueContainer);
    ~spec();
    
    std::string getId();
    clique* getClique();
    void setClique(clique *newClique);

    void merge(spec *second);
    void print();
};

class clique {
private:
    list<spec> content;
public:
    clique(spec* initialSpec);
    ~clique();

    list<spec>* getContentList();

    void merge(clique *second);
};

#endif /* SPEC_HPP */