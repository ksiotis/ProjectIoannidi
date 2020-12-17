#include <iostream>
#include <dirent.h>
#include <cstring>
#include <regex>
#include <sys/types.h>
#include <fstream>
#include <sstream>

#include "include/spec.hpp"
#include "include/hashtable.hpp"
#include "include/list.hpp"
#include "include/jsonParser.hpp"
#include "include/tf_idf.hpp"
#include "include/utilities.hpp"

// template <typename T>
// std::string T::* treeNode<T>::keyValue = &generic::id;

// using namespace std;
typedef std::string string;

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

        std::string line;
        getline(inputFile, line); //skip first line
        while (getline(inputFile, line)) { //for every line in file
            if (line.back() == *(char*)"1") {
                //line is ending in 1
                std::string id1,id2;
                id1 = line.substr(0, line.find(","));
                line.erase(0, id1.length()+1);

                id2 = line.substr(0, line.find(","));

                hashtab.getContentByKeyValue(id1)->merge(hashtab.getContentByKeyValue(id2)); 
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

int extractPairs(list<clique> &cliqueContainer, std::string csvOutputFile) {
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

    //initialize container structures
    hashtable<spec> hashtab(buckets);
    list<spec> specContainer;
    list<clique> cliqueContainer;

    //read directories to get ids and add them to the apropriate container structures
    read_directory(folder, hashtab, specContainer, cliqueContainer);

    //read csv file and reorganize the cliques accordingly
    if (readCSV(csv_file, hashtab) != 0) {
        return -1; //if it failed stop
    }
    if (extractPairs(cliqueContainer, csvOutputFile) != 0) {
        return -1; //if it failed stop
    }

    
    Index index = Index(buckets);
    json_index j_index = json_index("cammarkt.com//390.json",buckets);
    
    //json parser example
    jsonParser parser;
    jsonObject* json = parser.parse("Datasets/2013_camera_specs/cammarkt.com/390.json");

    std::string json_string = json->stringy();

    json_string = std::regex_replace(json_string, std::regex(R"([^A-Za-z ][\\n]*)"), " ");
    std::string stopwords[119] = {"a","able","about","across","after","all","almost","also","am","among","an","and","any","are","as","at","be","because","been","but","by","can","cannot","could","dear","did","do","does","either","else","ever","every","for","from","get","got","had","has","have","he","her","hers","him","his","how","however","i","if","in","into","is","it","its","just","least","let","like","likely","may","me","might","most","must","my","neither","no","nor","not","of","off","often","on","only","or","other","our","own","rather","said","say","says","she","should","since","so","some","than","that","the","their","them","then","there","these","they","this","tis","to","too","twas","us","wants","was","we","were","what","when","where","which","while","who","whom","why","will","with","would","yet","you","your"};
    for(int i=0; i<119;i++){
        json_string = std::regex_replace(json_string, std::regex("[^a-zA-Z]("+stopwords[i]+")[^a-zA-Z]"), " ");
    }
    json_string = removeWord(json_string,"  ");

    std::stringstream ss(json_string);
    string buf;

    int counter = 0;
    while (ss >> buf){
        std::cout << buf << std::endl;
        insert_word(&index,&j_index,buf);
        counter++;
    }
    std::cout << counter << "   THAT JSON HAS WORDS :P  " << index.get_words_counter() << std::endl;
    // std::cout << json_string << std::endl;
    //empty and delete container structures and dynamic data
    specContainer.emptyList(true);
    cliqueContainer.emptyList(true);
    delete json;
/********* END OF CSV PART **********/


    return 0;
}

// std::regex e ("\\[0-9]");
// std::regex_replace (std::back_inserter(result), s.begin(), s.end(), e, "$2");