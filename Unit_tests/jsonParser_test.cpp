#include <iostream>
#include <string>

#include "../include/acutest.h"
#include "../include/jsonParser.hpp"
#include "../include/utilities.hpp"


template <typename T>
std::string T::* treeNode<T>::keyValue = &unit_testing::id;

void test_hashtable_simple(void){
    unit_testing a = unit_testing("aaaa");
    unit_testing b = unit_testing("bbbb");
    hashtable<unit_testing> hashtab(3);
    hashtab.insert(&a);
    TEST_CHECK(hashtab.getTotalCount() == 1);
    hashtab.insert(&b);
    TEST_CHECK(hashtab.getTotalCount() == 2);
    TEST_CHECK(hashtab.isInside("g") == false && hashtab.isInside("bbbb"));
}

void test_hashtable_complex(void){
    hashtable<unit_testing> hashtab(3);

    for(int i = 0; i < 100; i++){
        unit_testing *a = new unit_testing(std::to_string(i));
        hashtab.insert(a);
    }
    TEST_CHECK(hashtab.getTotalCount() == 100);
    TEST_CHECK(hashtab.isInside("101") == false && hashtab.isInside("5"));
}




TEST_LIST = {
    {"Hashtable simple test" , test_hashtable_simple},
    {"Hashtable complex test" , test_hashtable_complex},
	{ NULL, NULL }
};