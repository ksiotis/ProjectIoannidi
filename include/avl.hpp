#ifndef AVL_HPP
#define AVL_HPP

#include <iostream>
#include "utilities.hpp"

template <typename T>
class treeNode {
private:
    T* content;
    treeNode* left;
    treeNode* right;
    int height;
public:
    static std::string T::* keyValue;
    
    treeNode(T* rec);
    ~treeNode();

    std::string getKey();
    int getHeight();
    int getHeight(treeNode* target);
    treeNode* getLeft();
    treeNode* getRight();
    T* getContent();

    void setHeight(int height);
    void setLeft(treeNode* node);
    void setRight(treeNode* node);

    treeNode* rotateLeft();
    treeNode* rotateRight();
    treeNode* insert(T* rec);

    void printTree();
};

template <typename T>
class avlTree {
private:
    treeNode<T>* root;
    int count;
public:
    avlTree();
    ~avlTree();

    int getCount();

    treeNode<T>* insert(T* rec);
    void printTree();
};

//~~~~~~~~~~~~~~~~~~~~~~treeNode~~~~~~~~~~~~~~~~~~~~~~~

template <typename T>
treeNode<T>::treeNode(T* rec) {
    content = rec;
    left = NULL;
    right = NULL;
    height = 0;
}

template <typename T>
treeNode<T>::~treeNode() {
    if (left != NULL) delete left;
    if (right != NULL) delete right;
}

template <typename T>
std::string treeNode<T>::getKey() { return content->*keyValue; }

template <typename T>
int treeNode<T>::getHeight() { return height; }

template <typename T>
int treeNode<T>::getHeight(treeNode* target) { return target == NULL ? -1 : target->getHeight(); }

template <typename T>
treeNode<T>* treeNode<T>::getLeft() { return left; }

template <typename T>
treeNode<T>* treeNode<T>::getRight() { return right; }

template <typename T>
T* treeNode<T>::getContent() { return content; }

template <typename T>
void treeNode<T>::setHeight(int newheight) { height = newheight; }

template <typename T>
void treeNode<T>::setLeft(treeNode* node) { left = node; }

template <typename T>
void treeNode<T>::setRight(treeNode* node) { right = node; }

template <typename T>
treeNode<T>* treeNode<T>::rotateLeft() {
    treeNode* temp = right;
    right = temp->getLeft();
    temp->setLeft(this);
    
    height = max(getHeight(right), getHeight(left)) + 1;
    temp->setHeight(max(getHeight(temp->right), getHeight(temp->left)) + 1);

    return temp;
}

template <typename T>
treeNode<T>* treeNode<T>::rotateRight() {
    treeNode* temp = left;
    left = temp->getRight();
    temp->setRight(this);

    height = max(getHeight(left), getHeight(right)) + 1;
    temp->setHeight(max(getHeight(temp->left), getHeight(temp->right)) + 1);

    return temp;
}

template <typename T>
treeNode<T>* treeNode<T>::insert(T* rec) {
    //a reccursive implementation of the basic avl tree insert and balancing
    std::string tkey = rec->*keyValue;
    treeNode* parent = this;
    int comparison = tkey.compare(content->*keyValue);

    // if (comparison == 0) { //if rec is supposed to be here
    //     content.insert(rec);
    // }
    if (comparison < 0) { //belongs to the left
        if (left != NULL)
            left = left->insert(rec);
        else
            left = new treeNode(rec);

        if (getHeight(left) - getHeight(right) == 2) {
            if (tkey.compare(left->getKey()) <= 0)
                parent = rotateRight(); //right rotation
            else {
                left = left->rotateLeft(); //left-right rotation
                parent = rotateRight();
            }
        }
    }
    else { //belongs to the right
        if (right != NULL)
            right = right->insert(rec);
        else
            right = new treeNode(rec);

        if (getHeight(right) - getHeight(left) == 2) {
            if(tkey.compare(right->getKey()) > 0)
                parent = rotateLeft(); //left rotation
            else {
                right = right->rotateRight(); //right-left rotation
                parent = rotateLeft();
            }
        }
    }

    height = max(getHeight(left), getHeight(right)) + 1; //for balancing
    return parent;
}

template <typename T>
void treeNode<T>::printTree() {
    for (int i = 0; i < 6-height; i++) {
        std::cout << " ";
    }
    std::cout << getKey() << std::endl;
    if (right != NULL) {
        std::cout << "\\";
        right->printTree();
    }
    if (left != NULL) {
        std::cout << "/";
        left->printTree();
    }
    std::cout << "|";
}

// // void treeNode<T>::getTotalCount(int &sum, std::string date1, std::string date2, std::string secondary) {
// //     /*get TOTAL COUNT OF RECORDS in current tree matching the given CRITERIA
// //       recursive function that adds the number in variable sum and then goes deeper*/
// //     bool flag = compareDates(date1, *key, date2);
// //     if (flag) {
// //         if (secondary == "")
// //             sum += content.getCount();
// //         else {
// //             listNode<record>* current = content.getStart();
// //             while (current != NULL) {
// //                 if (current->getContent()->getCountry() == secondary)
// //                     sum++;
// //                 current = current->getNext();
// //             }
// //         }
// //     }
// //     if (left != NULL && compareDates(date1, *key) < 0)  //if left node does not exist or date1 >= key no need to go left
// //         left->getTotalCount(sum, date1, date2, secondary);
// //     if (right != NULL && compareDates(*key, date2) < 0)  //if right node does not exist or date2 <= key no need to go right
// //         right->getTotalCount(sum, date1, date2, secondary);
// //     return;
// // }

// // void treeNode<T>::countAll(list<tempClass>& myList, bool searchCountries, std::string date1, std::string date2) {
// //     /*get TOTAL COUNT OF EACH COUNTRY records in current tree matching the given criteria
// //       recursive function that increases the count of each recorded secondary category (country/disease) in myList
// //       and then goes deeper*/
// //     if ((date1 == "" && date2 == "") || compareDates(date1, *key, date2) ) {
// //         // add to list
// //         listNode<record>* current = content.getStart();
// //         while (current != NULL) { //for each patient
// //             listNode<tempClass>* currentSecondary = myList.getStart();
// //             bool found = false;
// //             std::string* test;
// //             if (searchCountries) //search for specific secondary category based on searchCountries
// //                 test = current->getContent()->PgetCountry();
// //             else
// //                 test = current->getContent()->PgetPatientDisease();
// //             //search if secondary is already in myList
// //             while (currentSecondary != NULL) { 
// //                 if (searchCountries)
// //                     test = current->getContent()->PgetCountry();
// //                 else
// //                     test = current->getContent()->PgetPatientDisease();
// //                 //if it already exists, increase its count
// //                 if (*test == *(currentSecondary->getContent()->key)) {
// //                     found = true;
// //                     currentSecondary->getContent()->count++;
// //                     break;
// //                 }
// //                 currentSecondary = currentSecondary->getNext();
// //             }
// //             //if it doesn't exist add it in myList
// //             if (!found) {
// //                 tempClass* t = new tempClass(test);
// //                 myList.insert(t);
// //             }
// //             current = current->getNext();
// //         }
// //     }
// //     //call for lower nodes if they exist and could be valid if date1, date2 given
// //     // '''date == "" ||''' is kind of useless in current implementation but it makes it more usable in future use
// //     if (left != NULL && (date1 == "" || compareDates(date1, *key) < 0))
// //         left->countAll(myList, searchCountries, date1, date2);
// //     if (right != NULL && (date2 == "" || compareDates(*key, date2) < 0))
// //         right->countAll(myList, searchCountries, date1, date2);
// // }


// // void treeNode<T>::getCurrentCount(int& sum) {
// //     /*get total count of CURRENT PATIENTS in current tree
// //       recursive function that increases the count in variable sum
// //       and then goes deeper*/
// //     listNode<record>* curr = content.getStart();
// //     while (curr != NULL) {
// //         if (curr->getContent()->getExitDay() == "-")
// //             sum++;
// //         curr = curr->getNext();
// //     }
// //     if (left != NULL)
// //         left->getCurrentCount(sum);
// //     if (right != NULL)
// //         right->getCurrentCount(sum);
// // }

// //~~~~~~~~~~~~~~~~~~~~~~avlTree~~~~~~~~~~~~~~~~~~~~~~~

template <typename T>
avlTree<T>::avlTree() {
    root = NULL;
    count = 0;
}

template <typename T>
avlTree<T>::~avlTree() { if (root != NULL) delete root; }

template <typename T>
int avlTree<T>::getCount() { return count; }

template <typename T>
treeNode<T>* avlTree<T>::insert(T* rec) {
    if (rec == NULL)
        return NULL;
    else {
        if (root != NULL)
            root = root->insert(rec);
        else
            root = new treeNode<T>(rec);
    }
    count++;
    return root;
}

template <typename T>
void avlTree<T>::printTree() {
    std::cout << "Count: " << count << "\nPrinting Tree..." << std::endl;
    if (root != NULL)
        root->printTree();
    else
        std::cout << "Tree is empty." << count << std::endl;
}

#endif /* AVL_HPP */