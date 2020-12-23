#include <iostream>
#include <string>


#include "utilities.hpp"

int max(int first, int second) {
    return first > second ? first : second;
}

std::string id_to_path(std::string id, std::string folder){
    size_t p = -1; 
    while ((p = id.find("//")) != std::string::npos) 
            id.replace(p, 2, "/");
    return folder+"/"+id+".json";
}

std::string removeWord(std::string str, std::string word)  
{ 
    if (str.find(word) != std::string::npos) 
    { 
        size_t p = -1; 
  
        std::string tempWord = word + " "; 
        while ((p = str.find(word)) != std::string::npos) 
            str.replace(p, tempWord.length(), " "); 
  
        tempWord = " " + word; 
        while ((p = str.find(word)) != std::string::npos) 
            str.replace(p, tempWord.length(), " "); 
    }

    return str; 
} 



unit_testing::unit_testing(std::string x){
        id = x;
    };
    
unit_testing::~unit_testing(){};
