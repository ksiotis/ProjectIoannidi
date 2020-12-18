#include <string>

#include "utilities.hpp"

int max(int first, int second) {
    return first > second ? first : second;
}

std::string id_to_path(std::string id, std::string folder){
    size_t p = -1; 
    while ((p = id.find("//")) != std::string::npos) 
            id.replace(p, 2, "/");
    return folder+id+".json";
}


unit_testing::unit_testing(std::string x){
        id = x;
    };
    
unit_testing::~unit_testing(){};
