#ifndef TF_IDF
#define TF_IDF

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

#include "list.hpp"
#include "hashtable.hpp"
#include "utilities.hpp"
#include "jsonParser.hpp"
#include "matrix.hpp"

class json_indexObject: public generic{
private:
    int count; // number of time word appears inside the specific json
    float tf;
public:
    json_indexObject(std::string a);
    ~json_indexObject();

    void raiseCount();
    int getCount();

    void find_setTf(int n);
    float getTf();

};

class json_index: public generic{
private:
    hashtable<json_indexObject> *hash;
    list<json_indexObject> *container;
    unsigned int words_counter;
public:
    json_index(std::string name,int buckets);
    ~json_index();

    list<json_indexObject>* get_container();


    void insert(std::string key);

    bool isInside(std::string key);

    void raiseCount(std::string key);
    int getCount(std::string key);

    void fix_Tf();
    float getTf(std::string key);
};


// 
// 
// 
// 
// 

class IndexObject: public generic {
private:
    int nt; // nt as presented in Project's pdf file
    int dim; // dimension
    float idf; 
public:
    IndexObject(std::string a);
    IndexObject(std::string a,int dim, float idf); //used on reading a index.csv
    ~IndexObject();

    void raiseNt();
    int getNt();

    void setDim(int a);
    int getDim();

    void find_setIdf(int n);
    float getIdf();

    std::string getKey();

    // void print();
};

class Index {
private:
    hashtable<IndexObject> *hash;
    list<IndexObject> *container;
    unsigned int words_counter;
    unsigned int jsons_counter;
public:
    Index(int buckets);
    ~Index();

    list<IndexObject>* get_container();

    void raiseJsons_counter();
    unsigned int getJsons_counter();

    unsigned int get_words_counter();


    void insert(std::string key);
    void insertObject(IndexObject* obj);

    bool isInside(std::string key);

    void raiseNt(std::string key);
    int getNt(std::string key);

    void fix_dim(list<json_index>* json_index_container);
    int getDim(std::string key);

    void fix_idf();
    float getIdf(std::string key);

    float getAverageIdf();

};



//************** INDEX SEPERATION LINE ***************




// insert word on index/json
void insert_word(Index* index,json_index* json,std::string word);

//insert word on json only
void insert_word_json(json_index* json,std::string word);

//get a vector from existing json
void get_vector_tfidf(Index* index,json_index* json,float* vec);

// create a json and return vector
void make_get_vector_tfidf(Index* index,hashtable<json_index>* json_index_hashtable,list<json_index>* json_index_container,list<jsonObject>* jsonContainer,int buckets,std::string id,std::string path,float* vec);

// make index/jsons
int make_tf_idf(std::string csvPath,Index* index,hashtable<json_index>* json_index_hashtable,list<json_index>* json_index_container,list<jsonObject>* jsonContainer,int buckets, std::string path, int lines = __INT_MAX__);

// returns 1,0 base on csv and ids' absolute difference
int* transform_csv_to_vector(std::string csvPath,Index* index,hashtable<json_index>* json_index_hashtable,list<json_index>* json_index_container,list<jsonObject>* jsonContainer,int buckets,std::string json_path,matrix* training,int end_line,int start_line = 0);

// extract index to file
void write_out_index(Index* index,std::string csvOutputFile);

// read index from file
int read_index_csv(Index* index,std::string filename);
#endif /* SPEC_HPP */
