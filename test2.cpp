#include <iostream>
#include <string>

#include "include/spec.hpp"
#include "include/hashtable.hpp"

template <typename T>
std::string T::* treeNode<T>::keyValue = &spec::id;

int main() {

    hashtable<spec> hashtab(21);
    spec** table = new spec*[100];
    for (int i = 0; i < 100; i++) {
        std::cout << std::to_string(i) << std::endl;
        table[i] = new spec(std::to_string(i+1));
        hashtab.insert(table[i]);
    }


    spec temp("15");
    hashtab.insert(&temp);

    hashtab.printAll();
}