#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <regex>



#include "tf_idf.hpp"
#include "list.hpp"
#include "hashtable.hpp"
#include "jsonParser.hpp"


Index::Index(int buckets){
    hash = new hashtable<IndexObject>(buckets);
    container = new list<IndexObject>();
    words_counter = 0;
    jsons_counter = 0;
}

Index::~Index(){
    container->emptyList(true);
    delete container;
    delete hash;
}

void Index::raiseJsons_counter(){
    jsons_counter++;
}

unsigned int Index::getJsons_counter(){
    return jsons_counter;
}


unsigned int Index::get_words_counter(){
    return words_counter;
}

void Index::insert(std::string key){
    IndexObject *in = new IndexObject(key);
    container->insert(in);
    hash->insert(in);
    words_counter++;
}

bool Index::isInside(std::string key){
    return hash->isInside(key);
}

void Index::raiseNt(std::string key){
    hash->getContentByKeyValue(key)->raiseNt();
}

int Index::getNt(std::string key){
    return hash->getContentByKeyValue(key)->getNt();
}

int Index::getDim(std::string key){
    return hash->getContentByKeyValue(key)->getDim();
}

float Index::getIdf(std::string key){
    return hash->getContentByKeyValue(key)->getIdf();
}

void fix_dim_recursive(treeNode<IndexObject> *currentNode, int &counter) {
    treeNode<IndexObject> *left = currentNode->getLeft();
    treeNode<IndexObject> *right = currentNode->getRight();
    if (left != NULL) {
        fix_dim_recursive(left, counter);
    }

    currentNode->getContent()->setDim(counter++);

    if (right != NULL) {
        fix_dim_recursive(right, counter);
    }
}

void Index::fix_dim(){
    /*
        Set a counter to zero
        Go to each node of all the avl_nodes
        run node->getcontent()->set_dim(counter); on that node
        (raise counter on for the next node)
    */
    int counter = 0;

    int buckets = hash->getBucketNumber();
    for (int i = 0; i < buckets; i++) {
        avlTree<IndexObject> *currentTree = hash->getTree(i);
        if(currentTree != NULL) {
            fix_dim_recursive(currentTree->getRoot(), counter);
        }
    }

}

void Index::fix_idf(){
    listNode<IndexObject> *current = container->getStart();
    while (current != NULL) {
        current->getContent()->find_setIdf(jsons_counter);
        current = current->getNext();
    }
}

float Index::getAverageIdf(){
    listNode<IndexObject> *current = container->getStart();
    float avg=0.0;
    while (current != NULL) {
        avg += current->getContent()->getIdf();
        current = current->getNext();
    }
    return avg/words_counter;
}


//~~~~~~~~~~~~~~~~~~~IndexObject~~~~~~~~~~~~~~~~~~~~~~~~~~

IndexObject::IndexObject(std::string a):generic(a) {
    nt = 1;
    dim = -1;
    idf = -0.5;
}

IndexObject::~IndexObject(){}

std::string IndexObject::getKey(){
    return id;
}

void IndexObject::raiseNt(){nt++;}
int IndexObject::getNt(){return nt;}



void IndexObject::setDim(int a){dim = a;}
int IndexObject::getDim(){return dim;}

void IndexObject::find_setIdf(int n){idf = log10(float(n)/nt);}
float IndexObject::getIdf(){return idf;}

// void IndexObject::print(){std::cout << id << std::endl;}


//~~~~~~~~~~~~~~~~~~~json_index~~~~~~~~~~~~~~~~~~~~~~~~~~

json_index::json_index(std::string name,int buckets):generic(name){
    hash = new hashtable<json_indexObject>(buckets);
    container = new list<json_indexObject>();
    words_counter = 0;
}

json_index::~json_index(){
    container->emptyList(true);
    delete container;
    delete hash;
}

list<json_indexObject>* json_index::get_container(){ 
    return container; 
}

void json_index::insert(std::string key){
    json_indexObject *in = new json_indexObject(key);
    container->insert(in);
    hash->insert(in);
    words_counter++;
}

bool json_index::isInside(std::string key){
    return hash->isInside(key);
}

void json_index::raiseCount(std::string key){
    words_counter++;
    hash->getContentByKeyValue(key)->raiseCount();
}

int json_index::getCount(std::string key){
    return hash->getContentByKeyValue(key)->getCount();
}

void json_index::fix_Tf(){
    listNode<json_indexObject> *current = container->getStart();
    while (current != NULL) {
        current->getContent()->find_setTf(words_counter);
        current = current->getNext();
    }
}

float json_index::getTf(std::string key){
    return hash->getContentByKeyValue(key)->getTf();
}


//~~~~~~~~~~~~~~~~~~~json_indexObject~~~~~~~~~~~~~~~~~~~~~~~~~~

json_indexObject::json_indexObject(std::string a):generic(a) {
    count = 1;
    tf = 0;
}

json_indexObject::~json_indexObject(){}

void json_indexObject::raiseCount(){count++;}
int json_indexObject::getCount(){return count;}

void json_indexObject::find_setTf(int n){tf = float(count)/n;}
float json_indexObject::getTf(){return tf;}




void insert_word(Index* index,json_index* json,std::string word){
    if(json->isInside(word)){
        json->raiseCount(word);
    }else{
        json->insert(word);
        if(index->isInside(word)){
            index->raiseNt(word);
        }else{
            index->insert(word);
        }
    }
}

void get_vector_tfidf(Index* index,json_index* json,float* vec){
    std::string word;
    int dimension;
    listNode<json_indexObject> *current = json->get_container()->getStart();
    while (current != NULL) {
        word = current->getContent()->getId();
        // std::cout << word;
        dimension = index->getDim(word);
        // std::cout << " dim: " << dimension << ", count:  " << current->getContent()->getCount() << ", idf: " << index->getIdf(word) << std::endl;
        vec[dimension] = current->getContent()->getTf() * index->getIdf(word);
        current = current->getNext();
    }
}

void insert_json(Index* index,hashtable<json_index>* json_index_hashtable,list<json_index>* json_index_container,list<jsonObject>* jsonContainer,int buckets,std::string id,std::string path){
    std::string stopwords[119] = {"a","able","about","across","after","all","almost","also","am","among","an","and","any","are","as","at","be","because","been","but","by","can","cannot","could","dear","did","do","does","either","else","ever","every","for","from","get","got","had","has","have","he","her","hers","him","his","how","however","i","if","in","into","is","it","its","just","least","let","like","likely","may","me","might","most","must","my","neither","no","nor","not","of","off","often","on","only","or","other","our","own","rather","said","say","says","she","should","since","so","some","than","that","the","their","them","then","there","these","they","this","tis","to","too","twas","us","wants","was","we","were","what","when","where","which","while","who","whom","why","will","with","would","yet","you","your"};
    if(!json_index_hashtable->isInside(id)){
        // Parse the json
        jsonParser parser;
        // std::cout << "jsonContainer insert " << path1 << std::endl;
        path=id_to_path(id,path);
        jsonObject* json = parser.parse(path);

        // Clear data
        std::string json_string = json->stringy();
        json_string = std::regex_replace(json_string, std::regex(R"([^A-Za-z ][\\n]*)"), " ");
            for(int i=0; i<119;i++){
            json_string = std::regex_replace(json_string, std::regex("[^a-zA-Z]("+stopwords[i]+")[^a-zA-Z]"), " ");
        }
        json_string = removeWord(json_string,"  ");


        // Make json_index
        json_index* j_index = new json_index(id,buckets);
        // feed index
        std::stringstream ss(json_string);
        std::string buf;
        while (ss >> buf){
            insert_word(index,j_index,buf);
        }
        j_index->fix_Tf();
        index->raiseJsons_counter();
        index->fix_idf();
        // Sace data for future use
        jsonContainer->insert(json);
        json_index_container->insert(j_index);
        json_index_hashtable->insert(j_index);

        index->fix_dim();
    }
}

void make_get_vector_tfidf(Index* index,hashtable<json_index>* json_index_hashtable,list<json_index>* json_index_container,list<jsonObject>* jsonContainer,int buckets,std::string id,std::string path,float* vec){
    std::string stopwords[119] = {"a","able","about","across","after","all","almost","also","am","among","an","and","any","are","as","at","be","because","been","but","by","can","cannot","could","dear","did","do","does","either","else","ever","every","for","from","get","got","had","has","have","he","her","hers","him","his","how","however","i","if","in","into","is","it","its","just","least","let","like","likely","may","me","might","most","must","my","neither","no","nor","not","of","off","often","on","only","or","other","our","own","rather","said","say","says","she","should","since","so","some","than","that","the","their","them","then","there","these","they","this","tis","to","too","twas","us","wants","was","we","were","what","when","where","which","while","who","whom","why","will","with","would","yet","you","your"};
    if(!json_index_hashtable->isInside(id)){
        // Parse the json
        jsonParser parser;
        // std::cout << "jsonContainer insert " << path1 << std::endl;
        path=id_to_path(id,path);
        jsonObject* json = parser.parse(path);

        // Clear data
        std::string json_string = json->stringy();
        json_string = std::regex_replace(json_string, std::regex(R"([^A-Za-z ][\\n]*)"), " ");
            for(int i=0; i<119;i++){
            json_string = std::regex_replace(json_string, std::regex("[^a-zA-Z]("+stopwords[i]+")[^a-zA-Z]"), " ");
        }
        json_string = removeWord(json_string,"  ");


        // Make json_index
        json_index* j_index = new json_index(id,buckets);
        // feed index
        std::stringstream ss(json_string);
        std::string buf;
        while (ss >> buf){
            j_index->insert(buf);
        }
        j_index->fix_Tf();

        // Sace data for future use
        jsonContainer->insert(json);
        json_index_container->insert(j_index);
        json_index_hashtable->insert(j_index);
    }
    return get_vector_tfidf(index,json_index_hashtable->getContentByKeyValue(id),vec);
}

int make_tf_idf(std::string csvPath,Index* index,hashtable<json_index> *json_index_hashtable,list<json_index>* json_index_container,list<jsonObject>* jsonContainer,int buckets, std::string path, int lines) {
    std::ifstream inputFile(csvPath);
    std::string stopwords[119] = {"a","able","about","across","after","all","almost","also","am","among","an","and","any","are","as","at","be","because","been","but","by","can","cannot","could","dear","did","do","does","either","else","ever","every","for","from","get","got","had","has","have","he","her","hers","him","his","how","however","i","if","in","into","is","it","its","just","least","let","like","likely","may","me","might","most","must","my","neither","no","nor","not","of","off","often","on","only","or","other","our","own","rather","said","say","says","she","should","since","so","some","than","that","the","their","them","then","there","these","they","this","tis","to","too","twas","us","wants","was","we","were","what","when","where","which","while","who","whom","why","will","with","would","yet","you","your"};
    try {
        if (inputFile.is_open() == false) {
            throw "Can't open file!";
        }

        int currentLine = 0;
        std::string line;
        getline(inputFile, line); //skip first line
        while (currentLine++ < lines && getline(inputFile, line)) { //for every line in file

            if(line.back()!= '0' && line.back()!= '1'){
                std::cout << "Bad line: \"" << line << '"' << std::endl;
            }else{
                std::string id1,id2,path1,path2;
                id1 = line.substr(0, line.find(","));
                path1=id_to_path(id1,path);
                line.erase(0, id1.length()+1);
                id2 = line.substr(0, line.find(","));
                path2=id_to_path(id2,path);
                
                if(!json_index_hashtable->isInside(id1)){
                    // Parse the json
                    jsonParser parser;
                    // std::cout << "jsonContainer insert " << path1 << std::endl;
                    jsonObject* json = parser.parse(path1);

                    // Clear data
                    std::string json_string = json->stringy();
                    json_string = std::regex_replace(json_string, std::regex(R"([^A-Za-z ][\\n]*)"), " ");
                     for(int i=0; i<119;i++){
                        json_string = std::regex_replace(json_string, std::regex("[^a-zA-Z]("+stopwords[i]+")[^a-zA-Z]"), " ");
                    }
                    json_string = removeWord(json_string,"  ");


                    // Make json_index
                    json_index* j_index = new json_index(id1,buckets);
                    // feed index
                    std::stringstream ss(json_string);
                    std::string buf;
                    while (ss >> buf){
                        insert_word(index,j_index,buf);
                    }
                    j_index->fix_Tf();

                    index->raiseJsons_counter();
                    // Save data for future use
                    jsonContainer->insert(json);
                    json_index_container->insert(j_index);
                    json_index_hashtable->insert(j_index);
                }

                if(!json_index_hashtable->isInside(id2)){
                    // Parse the json
                    jsonParser parser2;
                    // std::cout << "jsonContainer insert " << path2 << std::endl;
                    jsonObject* json2 = parser2.parse(path2);

                    // Clear data
                    std::string json_string2 = json2->stringy();
                    json_string2 = std::regex_replace(json_string2, std::regex(R"([^A-Za-z ][\\n]*)"), " ");
                     for(int i=0; i<119;i++){
                        json_string2 = std::regex_replace(json_string2, std::regex("[^a-zA-Z]("+stopwords[i]+")[^a-zA-Z]"), " ");
                    }
                    json_string2 = removeWord(json_string2,"  ");

                    // Make json_index
                    json_index* j_index2 = new json_index(id2,buckets);
                    // feed index
                    std::stringstream ss2(json_string2);
                    std::string buf2;
                    while (ss2 >> buf2){
                        insert_word(index,j_index2,buf2);
                    }
                    j_index2->fix_Tf();

                    index->raiseJsons_counter();
                    // Save data for future use
                    jsonContainer->insert(json2);
                    json_index_container->insert(j_index2);
                    json_index_hashtable->insert(j_index2);
                }

            }

        }
        index->fix_idf();
        index->fix_dim();
    }
    catch (const char* e) {
        std::cout << "File Error! " << e << std::endl;
        inputFile.close();
        return -1;
    }
    return 0;
}

int* transform_csv_to_vector(std::string csvPath,Index* index,hashtable<json_index>* json_index_hashtable,matrix* training,int lines){
    std::ifstream inputFile(csvPath);
    if (inputFile.is_open() == false) {
            throw "Can't open file!";
    }

    int vec_count = index->get_words_counter();
    int *y = new int[vec_count];

    int currentLine = 0;
    std::string line;
    getline(inputFile, line); //skip first line
    while (currentLine < lines && getline(inputFile, line)) { //for every line in file
        if(line.back()!= '0' && line.back()!= '1'){
            std::cout << "Bad line: \"" << line << '"' << std::endl;
        }else{
            if(line.back() == '0'){
                y[currentLine] = 0;
            }else{
                y[currentLine] = 1;
            }



            std::string id1,id2;
            id1 = line.substr(0, line.find(","));
            line.erase(0, id1.length()+1);
            id2 = line.substr(0, line.find(","));
            

            float json1[vec_count],json2[vec_count];
            for (int i = 0; i < vec_count; i++) {
                json1[i] = 0.0;
                json2[i] = 0.0;
            }
            get_vector_tfidf(index,json_index_hashtable->getContentByKeyValue(id1),json1);
            get_vector_tfidf(index,json_index_hashtable->getContentByKeyValue(id2),json2);
            for (int i = 0; i < vec_count; i++) {
                training->table[currentLine][i] = json1[i] > json2[i] ? json1[i] - json2[i] : json2[i] - json1[i];
            }
        }
        currentLine++;
    }
    return y;
}