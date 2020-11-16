#include <iostream>
#include <string>

#include "../include/acutest.h"
#include "../include/avl.hpp"
#include "../include/utilities.hpp"


template <typename T>
std::string T::* treeNode<T>::keyValue = &unit_testing::id;

void test_tree_simple(void){
    unit_testing a = unit_testing("aaaa");
    unit_testing b = unit_testing("bbbb");
    avlTree<unit_testing> tree = avlTree<unit_testing>();
    tree.insert(&a);
    TEST_CHECK(tree.getCount() == 1);
    tree.insert(&b);
    TEST_CHECK(tree.getCount() == 2);
    TEST_CHECK(tree.isInside("g") == false && tree.isInside("bbbb"));
}

void test_tree_complex(void){
    avlTree<unit_testing> tree = avlTree<unit_testing>();

    for(int i = 0; i < 100; i++){
        unit_testing *a = new unit_testing(std::to_string(i));
        tree.insert(a);
    }
    TEST_CHECK(tree.getCount() == 100);
    TEST_CHECK(tree.isInside("101") == false && tree.isInside("5"));
}




TEST_LIST = {
    {"Tree simple test" , test_tree_simple},
    {"Tree complex test" , test_tree_complex},
	{ NULL, NULL }
};