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
    // static std::string T::* getId();
    
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
    bool isInside(std::string testkey);
    T* getContentByKeyValue(std::string testkey);

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
    bool isInside(std::string testkey);
    bool isInside(T* rec);
    T* getContentByKeyValue(std::string testkey);
    
    treeNode<T>* insert(T* rec);
    void printTree();

    //part 2
    treeNode<T> *getRoot();
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
std::string treeNode<T>::getKey() { return content->getId(); }

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
    std::string tkey = rec->getId();
    treeNode* parent = this;
    int comparison = tkey.compare(content->getId());

    // if (comparison == 0) { //if rec is supposed to be here
    //     content.insert(rec);
    // }
    if (comparison < 0) { //belongs to the left
        if (left != NULL)
            left = left->insert(rec);
        else
            left = new treeNode(rec);

        if (getHeight(left) - getHeight(right) == 2) {
            if (tkey <= left->getKey())
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
            if(tkey > right->getKey())
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
bool treeNode<T>::isInside(std::string testkey) {
    std::string mykey = getKey();
    if (mykey < testkey) {
        if (right != NULL)
            return right->isInside(testkey);
        else
            return false;
    }
    else if (mykey > testkey) {
        if (left != NULL)
            return left->isInside(testkey);
        else
            return false;
    }
    else
        return true;
}

template <typename T>
T* treeNode<T>::getContentByKeyValue(std::string testkey) {
    std::string mykey = getKey();
    if (mykey < testkey) {
        if (right != NULL)
            return right->getContentByKeyValue(testkey);
        else
            return NULL;
    }
    else if (mykey > testkey) {
        if (left != NULL)
            return left->getContentByKeyValue(testkey);
        else
            return NULL;
    }
    else
        return getContent();
}

template <typename T>
void treeNode<T>::printTree() {
    for (int i = 0; i < height; i++) {
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
bool avlTree<T>::isInside(std::string testkey) {
// is something equal to testkey in the tree?
    if (root != NULL)
        return root->isInside(testkey);
    else
        return false;
}

template <typename T>
bool avlTree<T>::isInside(T* rec) {
/* is something equal to the key of rec in the tree?
treeNode<T>::getId() is required to be set to the key of rec*/
    std::string mykey = rec->getId();
    return isInside(mykey);
}

template <typename T>
T* avlTree<T>::getContentByKeyValue(std::string testkey) {
/*get pointer to item with getId() of testkey
  item must have function getKey*/
    if (root != NULL)
        return root->getContentByKeyValue(testkey);
    else
        return NULL;
}

template <typename T>
void avlTree<T>::printTree() {
/*for debugging purposes*/
    if (root != NULL)
        root->printTree();
    else
        std::cout << "Tree is empty." << count << std::endl;
}

template <typename T>
treeNode<T> *avlTree<T>::getRoot() { return root; }


#endif /* AVL_HPP */
