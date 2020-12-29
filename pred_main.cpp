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

int main(int argc, char** argv) {
    /*input arguments should be:
        path to datasets folder
        path to csv prediction file
        path to csv output file
        path to csv index file
        path to model_path file
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
    std::string index_csv = argv[4];
    std::string model_path = argv[5];
    int buckets = 11;
    if (argc == 7) {
        buckets = atoi(argv[6]);
        if (buckets <= 0) {
            std::cerr << "Bucket number should be a positive integer!" << std::endl;
            return -1;
        }
    }
    
    //count lines
    int lines = 0;

    std::ifstream input;
    input.open(csv_file);
    if (!input.is_open()) {
        std::cerr << "Cant open csv file!" << std::endl;
        return -2;
    }
    std::string temp;
    while (std::getline(input, temp)) {
        lines++;
    }
    input.close();
    
    Index index(buckets);
    read_index_csv(&index, index_csv);
    hashtable<json_index> json_index_hashtable(buckets);
    list<json_index> json_index_container;
    list<jsonObject> jsonContainer;

    logistic_regression *lr = logistic_regression::loadModel(argv[5]);


    unsigned int vec_count = index.get_words_counter();
    int *tst;
    matrix test(lines, vec_count);

    
    
    tst = transform_csv_to_vector(csv_file,&index,&json_index_hashtable,&json_index_container,&jsonContainer,buckets,folder,&test,lines);

    matrix *predictions = lr->predict(test);

    std::ofstream output;
    output.open(csvOutputFile);
    if (!output.is_open()) {
        std::cerr << "Cant open output file" << std::endl;
    }
    input.open(csv_file);
    if (!input.is_open()) {
        std::cerr << "Cant open output file" << std::endl;
    }
    std::string line;
    std::getline(input,line);
    int i=0;
    while (std::getline(input,line) && i++ < lines){
        line.back() = predictions->table[i][0] < 0.5f ? '0' : '1';
        output << line << std::endl;
    }
    
    delete predictions;
    delete[] tst;


    //empty and delete container structures and dynamic data
    jsonContainer.emptyList(true);
    json_index_container.emptyList(true);
/********* END OF CSV PART **********/
    return 0;
}
