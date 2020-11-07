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
    cout << path << endl;
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        if( strcmp(dp->d_name,".") == 0 || strcmp(dp->d_name,"..") == 0){
            continue;
        }
        path2 = path + "/" + dp->d_name;
        dirp2 = opendir(path2.c_str());
        if(dirp2 != 0){
            read_directory(path2);
        }
        cout << (dp->d_name) << endl;
    }
    closedir(dirp);
}

int main() {
    cout << "Hello World!" << endl;
    read_directory("./Datasets");
    return 0;
}


