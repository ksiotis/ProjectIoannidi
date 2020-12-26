#include <iostream>
#include <string>

#include "../include/acutest.h"
#include "../include/matrix.hpp"
#include "../include/utilities.hpp"

void test_matrix_simple(void){
    matrix my(3,3);
    for (int i = 0; i < 9; i++) {
        my.table[i/3][i%3] = i+1;
    }
    TEST_CHECK(my.getRows() == 3 && my.getColumns() == 3);
    TEST_CHECK(my.table != NULL && my.table[2][2] == 9);
}

void test_matrix_dot(void){
    matrix test1(3,3);
    for (int i = 0; i < 9; i++) {
        test1.table[i/3][i%3] = i+1;
    }
    /*  1 2 3
        4 5 6
        7 8 9*/

    matrix test2(1,3);
    for (int i = 0; i < 3; i++) {
        test2.table[0][i%3] = i+1;
    }
    // 1 2 3

    matrix *dot = matrix::dot(test1, test2);
    /*expected: 14
                32
                50*/

    TEST_CHECK(dot->getRows() == 3 && dot->getColumns() == 1);
    TEST_CHECK(dot->table[0][0] == 14 && dot->table[1][0] == 32 && dot->table[2][0] == 50);
}



TEST_LIST = {
    {"Matrix simple test" , test_matrix_simple},
    {"Matrix dot product test" , test_matrix_dot},
	{ NULL, NULL }
};