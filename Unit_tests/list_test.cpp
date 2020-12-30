#include <iostream>
#include <string>

#include "../include/acutest.h"
#include "../include/list.hpp"


void test_nodes(void){
    int a = 5,b = 10;
    listNode<int> *node1 = new listNode<int>(&a);
    listNode<int> *node2 = new listNode<int>(&b);
    node1->setNext(node2);
    TEST_CHECK(node1->getNext() == node2);
    TEST_CHECK(node1->getContent() == &a);
    TEST_CHECK(node1->getNext()->getContent() == &b);

    delete node1;
    delete node2;
}

void test_list(void){
    int *a = new int(5);
    int *b = new int(10);
    list<int> li = list<int>();
    li.insert(a);
    li.insert(b);
    TEST_CHECK(li.getCount() == 2);
    TEST_CHECK(li.search(5) == true);
    TEST_CHECK(li.search(7) == false);
    TEST_CHECK(li.search(10) == true);
    TEST_CHECK(li.removeStart() == a);
    li.emptyList(true);
    TEST_CHECK(li.getCount() == 0);
}

void test_merge(void){
    int *a = new int(5);
    int *b = new int(10);
    list<int> li1 = list<int>();
    list<int> li2 = list<int>();
    li1.insert(a);
    li2.insert(b);
    li1.merge(li1,li2);
    TEST_CHECK(li1.getCount() == 2);

    li1.emptyList(true);
}


TEST_LIST = {
	{ "Nodes", test_nodes },
    { "List", test_list},
    { "Merge_list" , test_merge},
	{ NULL, NULL }
};