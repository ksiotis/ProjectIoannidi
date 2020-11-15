#ifndef LIST_HPP
#define LIST_HPP

// #include <cstring>
#include <string>

template <typename T>
class listNode {
private:
    T* content;
    listNode* next;
public:
    listNode(T* rec);
    ~listNode();

    T* getContent();
    listNode<T>* getNext();

    void setNext(listNode<T>* newNext);
};

template <typename T>
class list {
private:
    listNode<T>* start;
    listNode<T>* end;
    int count;
public:
    list();
    ~list();

    int getCount();
    listNode<T>* getStart();
    listNode<T>* getEnd();
    void insert(T* myValue);

    void printAll();
    void emptyList(bool deleteContent);
    bool search(T);
    T *removeStart();
    T* getContentByValue(std::string testkey);

    static void merge(list<T>& a, list<T>&b);
};

//~~~~~~~~~~~~~~~~~~~listNode~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename T>
listNode<T>::listNode(T* rec) {
    content = rec;
    next = NULL;
}

template <typename T>
listNode<T>::~listNode() {
    if (next != NULL)
        delete next;
}

template <typename T>
T* listNode<T>::getContent() { return content; }

template <typename T>
listNode<T>* listNode<T>::getNext() { return next; }

template <typename T>
void listNode<T>::setNext(listNode<T>* newNext) { next = newNext; }

//~~~~~~~~~~~~~~~~~~~~~~~list~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename T>
list<T>::list() {
    start = NULL;
    end = NULL;
    count = 0;
}

template <typename T>
list<T>::~list() {
    if (start != NULL)
        delete start;
}

template <typename T>
int list<T>::getCount() { return count; }

template <typename T>
listNode<T>* list<T>::getStart() { return start; }

template <typename T>
listNode<T>* list<T>::getEnd() { return end; }

template <typename T>
void list<T>::insert(T* content) {
    listNode<T>* temp = new listNode<T>(content);
    if (start != NULL)
        end->setNext(temp);
    else
        start = temp;
    end = temp;
    count++;
}

template <typename T>
void list<T>::printAll() {
    listNode<T> *curr = start;
    std::cout << "Printing list..." << std::endl;
    for (int i = 0; i < count; ++i) {
        std::cout << *(curr->getContent()) << " ";
        curr = curr->getNext();
    }
    std::cout << std::endl;
}

template <typename T>
void list<T>::emptyList(bool deleteContent) {

    listNode<T> *curr = start;
    int tempcount = count;
    for (int i = 0; i < tempcount; ++i) {
        if (deleteContent) {
            // std::cout << "deleting " << curr->getContent() << ": " << *(curr->getContent()) << std::endl;
            delete curr->getContent();
        }
        curr = curr->getNext();
        count--;
    }
    delete start;
    start = NULL;
}

template <typename T>
bool list<T>::search(T test) {
    listNode<T> *curr = start;
    for (int i = 0; i < count; ++i) {
        if (*(curr->getContent()) == test)
            return true;
        curr = curr->getNext();
    }
    return false;
}

template <typename T>
T *list<T>::removeStart() {
    T *returnable;
    if (start != NULL) {
        listNode<T> *temp = start;
        start = start->getNext();
        count--;
        if (count == 0)
            end = NULL;
        
        returnable = temp->getContent();

        temp->setNext(NULL);
        delete temp;
    }
    else {
        returnable = NULL;
    }

    return returnable;
}

template <typename T>
T* list<T>::getContentByValue(std::string testkey) {

    listNode<T> *current = start;
    while (current != NULL) {
        if (testkey == current->getContent()->getKey())
            return current->getContent();
        else
            current = current->getNext();
    }
    return NULL;
}

template <typename T>
void list<T>::merge(list<T> &a, list<T> &b) {
    a.count += b.count;
    if (a.end != NULL) {
        a.end->setNext(b.start);
    }
    else {
        a.start = b.start; 
    }
    if (b.end != NULL)
        a.end = b.end;
    b.count=0;
    b.start = NULL;
    // b.end=NULL;
    // delete b;
}

#endif /* LIST_HPP */