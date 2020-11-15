#include <iostream>
#include <dirent.h>
#include <cstring>
#include <sys/types.h>
#include <fstream>
#include <sstream>

#include "include/spec.hpp"
#include "include/hashtable.hpp"
#include "include/list.hpp"
#include "include/jsonParser.hpp"

template <typename T>
std::string T::* treeNode<T>::keyValue = &spec::id;

// using namespace std;
typedef std::string string;

void read_directory(const string name, hashtable<spec> &hashtab, 
        list<spec> &specContainer, list<clique> &cliqueContainer)
{
    string path = name;
    string path2;
    DIR* dirp = opendir(name.c_str());
    DIR* dirp2;
    if(dirp == 0){
        std::cout << "no such dir  " << path << std::endl;
        return;
    }
    // cout << path << endl;
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        if( strcmp(dp->d_name,".") == 0 || strcmp(dp->d_name,"..") == 0){
            continue;
        }
        path2 = path + "/" + dp->d_name;
        dirp2 = opendir(path2.c_str());
        if(dirp2 != 0){ //if directory read it
            closedir(dirp2);
            read_directory(path2, hashtab, specContainer, cliqueContainer);
        }else { //if file

            // keep only the last folder + filename
            string delimiter = "/";

            size_t pos = 0;
            string token;
            while ((pos = path2.find(delimiter)) != string::npos) {
                token = path2.substr(0, pos);
                path2.erase(0, pos + delimiter.length());
            }
            path2 = token + "//" + path2;
            if((pos = path2.find(".json")) != string::npos){ // Remove .json extension
                path2.erase(pos,path2.length());

                spec *currentSpec = new spec(path2, cliqueContainer);
                specContainer.insert(currentSpec);
                hashtab.insert(currentSpec);
            }
        }
    }
    closedir(dirp);
}

int main() {
    hashtable<spec> hashtab(5);
    list<spec> specContainer;
    list<clique> cliqueContainer;

    read_directory("./Datasets", hashtab, specContainer, cliqueContainer);

    // hashtab.printAll();


/********* CSV PART **********/
    std::ifstream inputFile("Datasets/sigmod_medium_labelled_dataset.csv");
    try {
        if (inputFile.is_open() == false) {
            throw "Can't open file!";
        }

        std::string line;
        getline(inputFile, line); //skip first line
        while (getline(inputFile, line)) { //for every line in file
            if (line.back() == *(char*)"1") {
                //line is ending in 1
                std::string id1,id2;
                id1 = line.substr(0, line.find(","));
                line.erase(0, id1.length()+1);

                id2 = line.substr(0, line.find(","));
                // std::cout << id1 << "\t" << id2 << std::endl;

                hashtab.getContentByValue(id1)->merge(hashtab.getContentByValue(id2)); 
            }
        }
    }
    catch (const char* e) {
        std::cout << "File Error! " << e << std::endl;
        inputFile.close();
        return -1;
    }

    //~~~~~~~~debugging~~~~~~~~~~~

    //print pairs -> 3582 lines
    // listNode<clique> *current = cliqueContainer.getStart();
    // while (current != NULL) {
    //     current->getContent()->printPairs();
    //     current = current->getNext();
    // }

    //print all 
    

    
    // while (currentSpec != NULL) {
    //     if (currentSpec->getContent()->getClique() != targetClique)
    //         std::cout << "Bad clique: " << currentSpec->getContent()->getId() << std::endl;
    //     std::cout << currentSpec->getContent()->getId() << std::endl;
    //     currentSpec = currentSpec->getNext();
    // }
    // std::cout << std::endl;

    jsonParser parser;
    parser.parse("Datasets/2013_camera_specs/cammarkt.com/390.json");
    // parser.object.addProperty("1","ass");
    // parser.object.addProperty("2","ass2");
    // parser.object.addProperty("3","ass3");

    // parser.object.addaray("4");
    // parser.object.insert("4","4ass1");
    // parser.object.insert("4","4ass2");

    // std::cout << ((array*)parser.object.content.getContentByValue("4"))->getContent()->getStart()->getContent()->getValue();

    specContainer.emptyList(true);
    cliqueContainer.emptyList(true);
/********* END OF CSV PART **********/


    return 0;
}