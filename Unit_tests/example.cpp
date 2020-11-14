#include <iostream>
#include <string>

#include "../include/acutest.h"
#include "../include/avl.hpp"
#include "../include/hashtable.hpp"
#include "../include/list.hpp"
#include "../include/spec.hpp"
#include "../include/utilities.hpp"

void test_clique(void){
    clique *a = new clique(NULL);
    TEST_CHECK(a != NULL);
}

void test_list(void){
    list<int> *li = new list<int>();
    TEST_CHECK(li != NULL);
}

void test_insert(void){
    list<int> *li = new list<int>();
    int a = 2;
    li->insert(&a);
    TEST_CHECK(li->getCount() == 1);
    int b = 5;
    li->insert(&b);
    TEST_CHECK(li->getCount() == 2);
}


void test_max(void){
    int a = max(5,0);
    TEST_CHECK(a == 5);
}


TEST_LIST = {
	{ "clique()", test_clique },
    { "list()", test_list},
    {"list_insert()" , test_insert},
    { "max(int,int)", test_max },
	{ NULL, NULL }
};