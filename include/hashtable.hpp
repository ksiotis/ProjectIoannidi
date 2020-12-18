#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include "avl.hpp"

template <typename T>
class hashtable {
private:
    int bucketNumber;
    avlTree<T>** table;

    unsigned int hash(T* rec);
    unsigned int hash(std::string temp);
public:
    // std::string T::*key;
    /*pointer to data member of T, which is a string
    gets its value from treeNode<T>::getId()*/

    hashtable(int buckets);
    ~hashtable();

    int getBucketNumber();
    int getTotalCount();
    bool isInside(T* rec);
    bool isInside(std::string testkey);
    void insert(T* rec);
    T* getContentByKeyValue(std::string testkey);

    void printAll();

    //part 2
    avlTree<T>* getTree(int i);

};

template <typename T>
unsigned int hashtable<T>::hash(T* rec) {
/*return the hashed value of key in rec
requires this->key to be set to key field in typeof(rec)*/
    std::string temp = rec->getId();
    return hash(temp);
}

template <typename T>
unsigned int hashtable<T>::hash(std::string temp) {
//return the hashed value of given string
    unsigned int product = 1;
    for (int i = 0, lenght = temp.length(); i < lenght; ++i) {
        product *= temp[i];
    }
    return product % bucketNumber;
}

template <typename T>
hashtable<T>::hashtable(int buckets) {
    // key = treeNode<T>::getId();
    bucketNumber = buckets;
    table = new avlTree<T>*[bucketNumber];
    for (int i = 0; i < bucketNumber; i++) {
        table[i] = NULL;
    }
}

template <typename T>
hashtable<T>::~hashtable() {
    for (int i = 0; i < bucketNumber; ++i) {
        if (table[i] != NULL)
            delete table[i];
    }
    delete[] table;
}

template <typename T>
int hashtable<T>::getBucketNumber() { return bucketNumber; }

template <typename T>
int hashtable<T>::getTotalCount() { 
    int sum = 0;
    for (int i = 0; i < bucketNumber; i++) {
        if (table[i] != NULL)
            sum += table[i]->getCount();
    }
    return sum;
}

template <typename T>
bool hashtable<T>::isInside(T* rec) {
// is something equal to the key of rec inside?
    unsigned int bucket = hash(rec);
    if (table[bucket] != NULL)
        return table[bucket]->isInside(rec);
    else
        return false;
}

template <typename T>
bool hashtable<T>::isInside(std::string testkey) {
// is something equal to testkey inside?
    unsigned int bucket = hash(testkey);
    if (table[bucket] != NULL)
        return table[bucket]->isInside(testkey);
    else
        return false;
}

template <typename T>
void hashtable<T>::insert(T* rec) {
/*attempts to insert rec into the hashtable,
  if it exists => displays error,
  if not => inserts rec to the avl tree of apropriate bucket*/
    if (isInside(rec)) { //if keyvalue is already in a bucket...
        std::cout << "Attempted to insert identical id: " << rec->getId() << std::endl;
    }
    else { //if keyvalue is not in a bucket
        unsigned int bucket = hash(rec);
        if (table[bucket] == NULL)
            table[bucket]=new avlTree<T>();
        table[bucket]->insert(rec);
    }
}

template <typename T>
T* hashtable<T>::getContentByKeyValue(std::string testkey) {
/*get pointer to item with getId() of testkey
  item must have function getKey*/
    unsigned int bucket = hash(testkey);
    if (table[bucket] != NULL)
        return table[bucket]->getContentByKeyValue(testkey);
    else
        return NULL;
}

template <typename T>
void hashtable<T>::printAll() {
//for debugging purposes
    for (int i=0; i < bucketNumber; i++) {
        std::cout << "Tree " << std::to_string(i) << ":" << std::endl;
        if (table[i] != NULL)
            table[i]->printTree();
        else
            std::cout << "Empty" << std::endl;
        std::cout << "---------------------" << std::endl;
    }
}

template <typename T>
avlTree<T>* hashtable<T>::getTree(int i) {
    return table[i];
}

#endif /* HASHTABLE_HPP */