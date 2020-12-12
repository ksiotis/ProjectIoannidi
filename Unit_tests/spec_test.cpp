#include <iostream>
#include <string>

#include "../include/acutest.h"
#include "../include/spec.hpp"
#include "../include/utilities.hpp"
#include "../include/list.hpp"

template <typename T>
int actualCount(list<T> *mylist) {
    listNode<T> *current = mylist->getStart();
    int i = 0;
    while (current != NULL) {
        current = current->getNext();
        i++;
    }
    return i;
}

void spec_clique_test(void){
    list<clique> li = list<clique>();
    spec *firstSpec = new spec("spec", li);
    TEST_CHECK(li.getCount() == 1);

    spec *secondSpec = new spec("cry", li);
    TEST_CHECK(li.getCount() == 2);
    firstSpec->merge(secondSpec);
    TEST_CHECK(li.getStart()->getContent()->getContentList()->getCount() == 2);
    TEST_CHECK(li.getEnd()->getContent()->getContentList()->getCount() == 0);

}

void negative_list_clique_test(void) {
    list<clique> li;
    spec *spec1 = new spec("spec1", li);
    spec *spec2 = new spec("spec2", li);
    spec *spec3 = new spec("spec3", li);
    spec *spec4 = new spec("spec4", li);
    spec *spec5 = new spec("spec5", li);
    TEST_CHECK(li.getCount() == 4);

    spec1->unsimilar(spec2);
    int count1 = spec1->getClique()->getNegativeList()->getCount();
    int count2 = spec2->getClique()->getNegativeList()->getCount();
    TEST_CHECK(count1 == 1 && count1 == actualCount(spec1->getClique()->getNegativeList()));
    TEST_CHECK(count2 == 1 && count2 == actualCount(spec2->getClique()->getNegativeList()));

    spec1->unsimilar(spec3);
    count1 = spec1->getClique()->getNegativeList()->getCount();
    int count3 = spec3->getClique()->getNegativeList()->getCount();
    TEST_CHECK(count1 == 2 && count1 == actualCount(spec1->getClique()->getNegativeList()));
    TEST_CHECK(count3 == 1 && count3 == actualCount(spec3->getClique()->getNegativeList()));

    spec3->unsimilar(spec4);
    count3 = spec3->getClique()->getNegativeList()->getCount();
    int count4 = spec4->getClique()->getNegativeList()->getCount();
    TEST_CHECK(count3 == 2 && count1 == actualCount(spec1->getClique()->getNegativeList()));
    TEST_CHECK(count4 == 1 && count4 == actualCount(spec4->getClique()->getNegativeList()));

    spec1->merge(spec4);
    count1 = spec1->getClique()->getNegativeList()->getCount();
    int count1_content = spec1->getClique()->getContentList()->getCount();
    TEST_CHECK(spec1->getClique() == spec4->getClique());
    TEST_CHECK(count1_content == 2 && count1_content == actualCount(spec1->getClique()->getContentList()));
    TEST_CHECK(count1 == 2 && count1 == actualCount(spec1->getClique()->getNegativeList()));

    // spec5->unsimilar(spec2);
    // count1 = spec1->getClique()->getNegativeList()->getCount();
    // int count1_content = spec1->getClique()->getContentList()->getCount();
    // TEST_CHECK(spec1->getClique() == spec4->getClique());
    // TEST_CHECK(count1_content == 2 && count1_content == actualCount(spec1->getClique()->getContentList()));
    // TEST_CHECK(count1 == 2 && count1 == actualCount(spec1->getClique()->getNegativeList()));
}




TEST_LIST = {
    {"SPEC_CLIQUE", spec_clique_test},
	{"NEGATIVE_CLIQUE",negative_list_clique_test},
    { NULL, NULL }
};