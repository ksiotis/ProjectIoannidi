#include <iostream>
#include <dirent.h>
#include <cstring>
#include <sys/types.h>
#include <fstream>
#include <sstream>

// using namespace std;
typedef std::string string;

void read_directory(const string name)
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
            read_directory(path2);
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
                // cout << path2 << endl;
            }
        }
    }
    closedir(dirp);
}

int main() {
    // read_directory("./Datasets");



    std::ifstream inputFile("Datasets/sigmod_medium_labelled_dataset.csv");
    try {
        if (inputFile.is_open() == false) {
            throw "Can't open file!";
        }

        std::string line;
        while (getline(inputFile, line)) { //for every line in file
            if (line.back() == *(char*)"1") {
                //line is ending in 1
                std::string id1,id2;
                id1 = line.substr(0, line.find(","));
                line.erase(0, id1.length()+1);

                id2 = line.substr(0, line.find(","));
                std::cout << id1 << "\t" << id2 << std::endl;

                
            }
        }
    }
    catch (const char* e) {
        std::cout << "File Error! " << e << std::endl;
        inputFile.close();
        return -1;
    }

    return 0;
}