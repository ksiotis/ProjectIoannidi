#include <iostream>
#include <string>

#include "../include/acutest.h"
#include "../include/jsonParser.hpp"
#include "../include/utilities.hpp"

void test_property(void){
    property a("testKey", "testValue");
    property b("testKey'", "testValue'");
    TEST_CHECK(a.getKey() == "testKey" && a.getValue() == "testValue");
    TEST_CHECK(b.getKey() == "testKey'" && b.getValue() == "testValue'");
}

void test_array(void){
    array test_array("key_value");
    test_array.addValue("v1");
    test_array.addValue("v2");
    test_array.addValue("v3");
    test_array.addValue("v4");
    test_array.addValue("v5");

    property *p1 = test_array.getContent()->getContentByKeyValue("v0");
    property *p2 = test_array.getContent()->getContentByKeyValue("v2");

    TEST_CHECK(p1 == NULL);
    TEST_CHECK(p2->getValue() == "v2");
    TEST_CHECK(test_array.getContent()->getCount() == 5);
}

void test_jsonObject(void){
    jsonObject o;
    o.addaray("a1");
    o.addProperty("p1","v1");
    o.addProperty("p2","v2");
    o.addProperty("p3","v3");
    o.addaray("a2");
    o.addProperty("p4","v4");
    o.insert("a2","v5");

    data *p1 = o.content.getContentByKeyValue("p3");
    data *p2 = o.content.getContentByKeyValue("a2");

    TEST_CHECK(o.content.getCount() == 6);
    TEST_CHECK(((property*)p1)->getValue() == "v3");
    TEST_CHECK(((array*)p2)->getContent()->getCount() == 1);
    TEST_CHECK(((array*)p2)->getContent()->getContentByKeyValue("v5") != NULL);
}



TEST_LIST = {
    {"propery test" , test_property},
    {"array test" , test_array},
    {"jsonObject test" , test_jsonObject},
	{ NULL, NULL }
};