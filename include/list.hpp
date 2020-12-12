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
    T* getContentByKeyValue(std::string testkey);

    static void merge(list<T>& a, list<T>&b);

    //Part 2
    bool search(T*);
    void remove(T*);
    void remove(T*, T*);
};

//~~~~~~~~~~~~~~~~~~~listNode~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename T>
listNode<T>::listNode(T* rec) {
    content = rec;
    next = NULL;
}

template <typename T>
listNode<T>::~listNode() {}

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
    listNode<T> *curr = start;
    if (start != NULL) {
        listNode<T> *newcurr = curr->getNext();
        delete curr;
        curr = newcurr;
    }
}

template <typename T>
int list<T>::getCount() { return count; }

template <typename T>
listNode<T>* list<T>::getStart() { return start; }

template <typename T>
listNode<T>* list<T>::getEnd() { return end; }

template <typename T>
void list<T>::insert(T* content) {
//insert to the end of list
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
//print all elements in list, if content can be printed
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
/*empty list by deleting all listNodes,
  deleteContent is a flag of wheather to delete the content of pointers in nodes*/
    listNode<T> *curr = start;
    int tempcount = count;
    for (int i = 0; i < tempcount; ++i) {
        if (deleteContent) {
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
//is something equal to test inside?
    listNode<T> *curr = start;
    for (int i = 0; i < count; ++i) {
        if (*(curr->getContent()) == test)
            return true;
        curr = curr->getNext();
    }
    return false;
}

template <typename T>
bool list<T>::search(T *test) {
//is something equal to test inside?
    listNode<T> *curr = start;
    // for (int i = 0; i < count; ++i) {
    //     if (curr->getContent() == test)
    //         return true;
    //     curr = curr->getNext();
    // }
    while (curr != NULL) {
        if (curr->getContent() == test)
            return true;
        curr = curr->getNext();
    }
    return false;
}

template <typename T>
T *list<T>::removeStart() {
/*remove the starting node and return its pointer
  used for FIFO*/
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
T* list<T>::getContentByKeyValue(std::string testkey) {
/*get pointer to item with keyValue of testkey
  item must have function getKey*/
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
/*merge list b to list a, does NOT delete list b*/
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
    b.end=NULL;
    // delete b;
}

template <typename T>
void list<T>::remove(T* test) {
    listNode<T> *previous = NULL;
    listNode<T> *current = start();
    while (current != NULL) {
        if (current->getContent() == test) {
            if (previous != NULL)
                previous->setNext(current->getNext());
            else
                start = current->getNext();

            if (end == current)
                end = previous;
            
            current->setNext(NULL);
            delete current;
            break;
        }
        current = current->getNext();
    }
    return;
}

template <typename T>
void list<T>::remove(T* test, T* test2) {
    listNode<T> *previous = NULL;
    listNode<T> *current = start;
    char toBeFound = 2;
    while (toBeFound && (current != NULL)) {
        T *testCurr = current->getContent();
        if (testCurr == test || testCurr == test2) {
            if (previous == NULL) {
                start = current->getNext();
            }
            else {
                previous->setNext(current->getNext());
            }
            if (end == current)
                end = previous;
            toBeFound--;
            count--;

            listNode<T> *newcurrent = current->getNext();
            delete current;
            current = newcurrent;
            continue;
        }
        previous = current;
        current = current->getNext();
    }
    return;
}

#endif /* LIST_HPP */