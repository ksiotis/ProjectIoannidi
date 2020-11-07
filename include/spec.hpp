#ifndef SPEC_HPP
#define SPEC_HPP

#include <iostream>
#include <string>
class clique;

class spec {
private:
    std::string id;
    clique *myClique;
public:
    spec(std::string a) {
        id = a;
        myClique=NULL;
    }
    ~spec() {}
    void print() {
        std::cout << id << std::endl;
    }
};
#endif /* SPEC_HPP */