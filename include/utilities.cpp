#include <string>

#include "utilities.hpp"

int max(int first, int second) {
    return first > second ? first : second;
}

unit_testing::unit_testing(std::string x){
        id = x;
    };
    
unit_testing::~unit_testing(){};
