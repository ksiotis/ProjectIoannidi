#include <iostream>
#include <string>

#include "../include/acutest.h"
#include "../include/spec.hpp"
#include "../include/utilities.hpp"
#include "../include/list.hpp"

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





TEST_LIST = {
    {"SPEC_CLIQUE", spec_clique_test},
	{ NULL, NULL }
};