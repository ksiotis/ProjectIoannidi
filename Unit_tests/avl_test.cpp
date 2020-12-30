#include <iostream>
#include <string>

#include "../include/acutest.h"
#include "../include/avl.hpp"
#include "../include/utilities.hpp"


// template <typename T>
// std::string T::* treeNode<T>::keyValue = &unit_testing::id;

void test_tree_simple(void){
    generic a = generic("aaaa");
    generic b = generic("bbbb");
    avlTree<generic> tree = avlTree<generic>();
    tree.insert(&a);
    TEST_CHECK(tree.getCount() == 1);
    tree.insert(&b);
    TEST_CHECK(tree.getCount() == 2);
    TEST_CHECK(tree.isInside("g") == false && tree.isInside("bbbb"));
}

void test_tree_complex(void){
    avlTree<generic> tree = avlTree<generic>();

    for(int i = 0; i < 100; i++){
        generic *a = new generic(std::to_string(i));
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