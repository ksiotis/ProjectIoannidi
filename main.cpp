#include <iostream>
#include <dirent.h>
#include <cstring>
#include <sys/types.h>
#include <fstream>
#include <sstream>
#include <cstring>

#include "include/spec.hpp"
#include "include/hashtable.hpp"
#include "include/list.hpp"
#include "include/jsonParser.hpp"
#include "include/tf_idf.hpp"
#include "include/logistic_regression.hpp"

// template <typename T>
// std::string T::* treeNode<T>::keyValue = &generic::id;

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

int readCSV(std::string csvPath, hashtable<spec> &hashtab) {
    std::ifstream inputFile(csvPath);
    try {
        if (inputFile.is_open() == false) {
            throw "Can't open file!";
        }

        int counter = 0;

        std::string line;
        getline(inputFile, line); //skip first line
        while (getline(inputFile, line)) { //for every line in file
            // std::cout << line;
            counter++;
            if (line.back() == '1') {// a,b,1
                //line is ending in 1
                std::string id1, id2;
                id1 = line.substr(0, line.find(","));
                line.erase(0, id1.length()+1);
                id2 = line.substr(0, line.find(","));


                // std::cout << "(1)" << std::endl;
                hashtab.getContentByKeyValue(id1)->merge(hashtab.getContentByKeyValue(id2)); 
            }
            else if (line.back() == '0') {// a,b,0
                //line is ending in 0
                std::string id1,id2;
                id1 = line.substr(0, line.find(","));
                line.erase(0, id1.length()+1);
                id2 = line.substr(0, line.find(","));

                // std::cout << "(2)" << std::endl;
                hashtab.getContentByKeyValue(id1)->unsimilar(hashtab.getContentByKeyValue(id2));
            }
            else
                std::cout << "Bad line: \"" << line << '"' << std::endl;
        }

        return counter;
    }
    catch (const char* e) {
        std::cout << "File Error! " << e << std::endl;
        inputFile.close();
        return -1;
    }
}

int extractPositivePairs(list<clique> &cliqueContainer, std::string csvOutputFile) {
    try {
        //check if output file already exists
        std::ifstream outfile;
        outfile.open(csvOutputFile);
        if (outfile)
            throw "File already exists!";

        //create output file
        std::ofstream ofile;
        ofile.open(csvOutputFile);
        if (!ofile)
            throw "Cant't open new file";

        //write pairs to file
        listNode<clique> *current = cliqueContainer.getStart();
        while (current != NULL) {
            current->getContent()->writePairs(ofile);
            current = current->getNext();
        }
    }
    catch(const char* e) {
        std::cerr << e << '\n';
        return -1;
    }
    return 0;
}

int main(int argc, char** argv) {
    /*input arguments should be:
        path to datasets folder
        path to csv input file
        path to csv output file
        number of buckets (default 11)
    */

    //handle input arguments
    if (argc < 4) {
       std::cerr << "Not enough arguments!" << std::endl;
       return -1;
    }
    std::string folder = argv[1];
    std::string csv_file = argv[2];
    std::string csvOutputFile = argv[3];
    int buckets = 11;
    if (argc == 5) {
        buckets = atoi(argv[4]);
        if (buckets <= 0) {
            std::cerr << "Bucket number should be a positive integer!" << std::endl;
            return -1;
        }
    }


    jsonParser parser;
    //initialize container structures
    hashtable<spec> hashtab(buckets);
    list<spec> specContainer;
    list<clique> cliqueContainer;

    //read directories to get ids and add them to the apropriate container structures
    read_directory(folder, hashtab, specContainer, cliqueContainer);

    //read csv file and reorganize the cliques accordingly...
    //and get the number of lines in file to split into train,test,eval
    int lines = readCSV(csv_file, hashtab);
    if (lines < 0) {
        return -1; //if it failed stop
    }

    //out pairs to file
    if (extractPositivePairs(cliqueContainer, csvOutputFile) != 0) {
        return -1; //if it failed stop
    }

    Index index(buckets);
    hashtable<json_index> json_index_hashtable(buckets);
    list<json_index> json_index_container;
    list<jsonObject> jsonContainer;

    
    int trainSet = lines / 100 * 60;
    if(make_tf_idf(csv_file,&index,&json_index_hashtable,&json_index_container,&jsonContainer,buckets, folder, trainSet) != 0){
        std::cout << "Error make tfidf 1" << std::endl;
        return -2;
    }

    unsigned int vec_count = index.get_words_counter();
    int *y;
    
    matrix training(trainSet, vec_count);
    y = transform_csv_to_vector(csv_file,&index,&json_index_hashtable,&json_index_container,&jsonContainer,buckets,folder,&training,trainSet);

    logistic_regression lr(2.0f, vec_count);
    std::cout << lr.epoch(training, y) << std::endl;
    

    write_out_index(&index,"index.csv");
    //empty and delete container structures and dynamic data
    delete [] y;
    specContainer.emptyList(true);
    cliqueContainer.emptyList(true);
    jsonContainer.emptyList(true);
    json_index_container.emptyList(true);
/********* END OF CSV PART **********/
    return 0;
}
