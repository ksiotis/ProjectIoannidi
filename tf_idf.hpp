#ifndef TF_IDF
#define TF_IDF

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

#include "list.hpp"
#include "hashtable.hpp"


class Index {
private:
    hashtable<IndexObject> *hash;
    list<IndexObject> *container;
    unsigned int words_counter;
public:
    Index(int buckets);
    ~Index();

    void insert(std::string key);

    bool isInside(std::string key);

    void raiseNt(std::string key);
    int getNt(std::string key);

    void fix_dim(); // TODO
    int getDim(std::string key);

    void fix_idf(); // TODO
    int getIdf(std::string key);
};

class IndexObject {
private:
    std::string id;
    int nt; // nt as presented in Project's pdf file
    int dim; // dimension
    float idf; 
public:
    IndexObject(std::string a);
    ~IndexObject();
    
    std::string getWord();

    void raiseNt();
    int getNt();

    void setDim(int a);
    int getDim();

    void find_setIdf(int n);
    float getIdf();

    // void print();
};


//************** INDEX SEPERATION LINE ***************


class json_index {
private:
    std::string id;
    hashtable<json_indexObject> *hash;
    list<json_indexObject> *container;
    unsigned int words_counter;
public:
    json_index(std::string name,int buckets);
    ~json_index();

    std::string getId();

    void insert(std::string key);

    bool isInside(std::string key);

    void raiseCount(std::string key);
    int getCount(std::string key);

    void fix_Tf();
    float getTf(std::string key);
};

class json_indexObject {
private:
    std::string id;
    int count; // number of time word appears inside the specific json
    float tf;
public:
    json_indexObject(std::string a);
    ~json_indexObject();

    std::string getWord();

    void raiseCount();
    int getCount();

    void find_setTf(int n);
    float getTf();

};


#endif /* SPEC_HPP */