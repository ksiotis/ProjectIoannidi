#include <iostream>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>

using namespace std;

void read_directory(const string name)
{
    string path = name;
    string path2;
    DIR* dirp = opendir(name.c_str());
    DIR* dirp2;
    if(dirp == 0){
        cout << "no such dir  " << path << endl;
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
                cout << path2 << endl;
            }
        }
    }
    closedir(dirp);
}

int main() {
    cout << "Hello World!" << endl;
    read_directory("./Datasets");
    return 0;
}