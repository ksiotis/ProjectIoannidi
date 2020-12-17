#include <iostream>
#include <string>
#include <fstream>
#include <cmath>


#include "tf_idf.hpp"
#include "list.hpp"
#include "hashtable.hpp"


Index::Index(int buckets){
    hash = new hashtable<IndexObject>(buckets);
    container = new list<IndexObject>();
    words_counter=0;
}

Index::~Index(){
    container->emptyList(true);
}

unsigned int Index::get_words_counter(){
    return words_counter;
}

void Index::insert(std::string key){
    IndexObject *in = new IndexObject(key);
    container->insert(in);
    hash->insert(in);
    words_counter++;
}

bool Index::isInside(std::string key){
    return hash->isInside(key);
}

void Index::raiseNt(std::string key){
    hash->getContentByKeyValue(key)->raiseNt();
}

int Index::getNt(std::string key){
    return hash->getContentByKeyValue(key)->getNt();
}

int Index::getDim(std::string key){
    return hash->getContentByKeyValue(key)->getDim();
}

int Index::getIdf(std::string key){
    return hash->getContentByKeyValue(key)->getIdf();
}

//TODO ********************************************************************************************************
void fix_dim_recursive(treeNode<IndexObject> *currentNode, int &counter) {
    treeNode<IndexObject> *left = currentNode->getLeft();
    treeNode<IndexObject> *right = currentNode->getRight();
    if (left != NULL) {
        fix_dim_recursive(left, counter);
    }

    currentNode->getContent()->setDim(counter++);

    if (right != NULL) {
        fix_dim_recursive(right, counter);
    }
}

void Index::fix_dim(){
    /*
        Set a counter to zero
        Go to each node of all the avl_nodes
        run node->getcontent()->set_dim(counter); on that node
        (raise counter on for the next node)
    */
    int counter = 0;

    int buckets = hash->getBucketNumber();
    for (int i = 0; i < buckets; i++) {
        avlTree<IndexObject> *currentTree = hash->getTree(i);
        fix_dim_recursive(currentTree->getRoot(), counter);
    }

}

//TODO ********************************************************************************************************
void Index::fix_idf(){
    listNode<IndexObject> *current = container->getStart();
    while (current != NULL) {
        current->getContent()->find_setIdf(words_counter);
        current = current->getNext();
    }
}

//~~~~~~~~~~~~~~~~~~~IndexObject~~~~~~~~~~~~~~~~~~~~~~~~~~

IndexObject::IndexObject(std::string a){
    id = a;
    nt = 1;
    dim = -1;
    idf = -0.5;
}

IndexObject::~IndexObject(){}
std::string IndexObject::getWord(){ return id;}

void IndexObject::raiseNt(){nt++;}
int IndexObject::getNt(){return nt;}


void IndexObject::setDim(int a){dim = a;}
int IndexObject::getDim(){return dim;}

void IndexObject::find_setIdf(int n){idf = log10(float(n)/nt);}
float IndexObject::getIdf(){return idf;}

// void IndexObject::print(){std::cout << id << std::endl;}


//~~~~~~~~~~~~~~~~~~~json_index~~~~~~~~~~~~~~~~~~~~~~~~~~

json_index::json_index(std::string name,int buckets){
    id = name;
    hash = new hashtable<json_indexObject>(buckets);
    container = new list<json_indexObject>();
    words_counter = 0;
}

json_index::~json_index(){
    container->emptyList(true);
}

std::string json_index::getId(){
    return id;
}

void json_index::insert(std::string key){
    json_indexObject *in = new json_indexObject(key);
    container->insert(in);
    hash->insert(in);
    words_counter++;
}

bool json_index::isInside(std::string key){
    return hash->isInside(key);
}

void json_index::raiseCount(std::string key){
    words_counter++;
}

int json_index::getCount(std::string key){
    return hash->getContentByKeyValue(key)->getCount();
}

void json_index::fix_Tf(){
    listNode<json_indexObject> *current = container->getStart();
    while (current != NULL) {
        current->getContent()->find_setTf(words_counter);
        current = current->getNext();
    }
}

float json_index::getTf(std::string key){
    return hash->getContentByKeyValue(key)->getTf();
}


//~~~~~~~~~~~~~~~~~~~json_indexObject~~~~~~~~~~~~~~~~~~~~~~~~~~

json_indexObject::json_indexObject(std::string a){
    id = a;
    count = 1;
    tf = 0;
}

json_indexObject::~json_indexObject(){}
std::string json_indexObject::getWord(){ return id;}

void json_indexObject::raiseCount(){count++;}
int json_indexObject::getCount(){return count;}

void json_indexObject::find_setTf(int n){tf = float(count)/n;}
float json_indexObject::getTf(){return tf;}




void insert_word(Index* index,json_index* json,std::string word){
    if(json->isInside(word)){
        json->raiseCount(word);
    }else{
        json->insert(word);
        if(index->isInside(word)){
            index->raiseNt(word);
        }else{
            index->insert(word);
        }
    }
}