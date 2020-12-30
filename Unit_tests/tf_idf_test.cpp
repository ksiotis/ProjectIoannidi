#include <iostream>
#include <sstream>
#include <string>

#include "../include/acutest.h"
#include "../include/tf_idf.hpp"
#include "../include/utilities.hpp"


void tf_idf_create(void){
    Index index = Index(10);
    std::stringstream ss;
    std::string buf;

    json_index j1 = json_index("first",1);
    std::string testing_heap = "cat cat dog";
    ss.str(testing_heap);
    while (ss >> buf){
        insert_word(&index,&j1,buf);
    }
    ss.clear();
    j1.fix_Tf();
    index.raiseJsons_counter();

    TEST_CHECK(j1.getCount("cat") == 2);
    TEST_CHECK(j1.getTf("cat") == float(2.0/3) );



    json_index j2 = json_index("second",1);
    testing_heap = "dog mouse mouse";
    ss.str(testing_heap);
    while (ss >> buf){
        insert_word(&index,&j2,buf);
    }
    ss.clear();
    j2.fix_Tf();
    index.raiseJsons_counter();

    index.fix_idf();
    index.fix_dim();
    TEST_CHECK(index.getIdf("dog") == 0);
    TEST_CHECK(index.isInside("dogie") == false);
}




TEST_LIST = {
    {"tf_idf_create" , tf_idf_create},
	{ NULL, NULL }
};