#include <string>
#include <cstring>

#include <iostream>

#include "records.hpp"
#include "avl.hpp"
#include "hashtable.hpp"

typedef std::string string;

//~~~~~~~~~~~~~~~~~hashtable~~~~~~~~~~~~~~~~~
//private

unsigned int hashtable::hash(record* rec) {
    //return the hashed value of key in record
    string temp = (*rec).*key;
    return hash(temp);
}

unsigned int hashtable::hash(string temp) {
    //return the hashed value of given string
    unsigned int sum = 1;
    for (int i = 0, lenght = temp.length(); i < lenght; ++i) {
        sum *= temp[i];
    }
    return sum % bucketNumber;
}

bool hashtable::find(record* rec, char* &response) {
    /*Searches for the tree with record key value
      If found, returns true and response contains pointer to said tree
      If not found, returns false and response contains pointer to last bucket of current chain
      If bucket does not exist, response is NULL*/
    unsigned int bucket = hash(rec);
    char* current;
    char* next = table[bucket];
    do {
        current = next;

        if (current == NULL) {
            response = NULL;
            return false;
        }

        //search for record's tree
        int count = *(int*)current;
        for (int i = 0; i < count; i++) {
            string* testString;
            char* position = current + sizeof(int) + i * (sizeof(string*) + sizeof(avlTree*));
            memcpy(&testString, position, sizeof(string*));

            if (*testString == *rec.*key) {
                memcpy(&response, position + sizeof(string*), sizeof(avlTree*));
                return true;
            }
        }
        char* position = current + bucketSize - sizeof(char*);
        memcpy(&next, position, sizeof(char*));

    } while (next != NULL);

    //return if found and pointer to it
    response = current;
    return false;
}

char* hashtable::newBucket() {
    return (char*)calloc(bucketSize, 1);
}

//public

hashtable::hashtable(int buckets, int b, bool isCountryKey) {
    bucketNumber = buckets;
    bucketSize = b;
    // recordsInTable = new int[bucketNumber];
    // // table = new bucket*[bucketNumber];
    table = new char*[bucketNumber];
    for (int i = 0; i < bucketNumber; i++) {
        // recordsInTable[i] = 0;
        table[i] = NULL;
    }
    recordsPerBucket = (b - sizeof(int) - sizeof(char*))/(sizeof(char*) + sizeof(avlTree*));

    if (isCountryKey)
        key = &record::country;
    else
        key = &record::patientDisease;
}

hashtable::~hashtable() {
    for (int i = 0; i < bucketNumber; ++i) {
        char* current = table[i];
        while (current != NULL) {
            int count = *(int*)current;
            for (int j = 0; j < count; ++j) {
                char* test = current + sizeof(int) + sizeof(string*) + j * (sizeof(string*) + sizeof(avlTree*));
                avlTree* tree; 
                memcpy(&tree, test, sizeof(avlTree*));
                delete tree;
            }

            char* next = *(char**)(current + bucketSize - sizeof(char*));
            free(current);
            current = next;
        }
    }
    delete[] table;
}

int hashtable::getRecordsPerBucket() { return recordsPerBucket; }

void hashtable::insert(record* rec) {
    char* temp;
    if (find(rec, temp)) { //if keyvalue is already in a bucket...
        avlTree* targetTree = (avlTree*)temp;
        targetTree->insert(rec);//...add record to its tree
    }
    else { //if keyvalue is not in a bucket
        int bucket = hash(rec);//go and add it there
        if (temp == NULL) { //if bucket does not exist create it
            table[bucket] = newBucket();
            temp = table[bucket];
        }

        int count = *(int*)temp;
        if (count == recordsPerBucket) {
            char** next = (char**)(temp + bucketSize - sizeof(char*));
            *next = newBucket();
            temp = *next;
            count = *(int*)temp;
        }

        //add pointer to string and pointer to tree with values of current rec
        char* position = temp + sizeof(int) + count * (sizeof(string*) + sizeof(avlTree*));
        string* target = &(*rec.*key);
        memcpy(position, &target, sizeof(string*));

        position = position + sizeof(string*);
        avlTree* newTree = new avlTree();
        memcpy(position, &newTree, sizeof(avlTree*));

        newTree->insert(rec); //increase count
        *(int*)temp = count + 1;
    }
}

void hashtable::displayRecords(std::string date1, std::string date2) {
    /*for /globalDiseaseStats
      displays for all diseases the number of patients*/
    for (int j = 0; j < bucketNumber; ++j) { //for each bucket
        char* current;
        char* next = table[j]; //get the start of each chain
        do {
            current = next; //move forward

            if (current == NULL) { //if reached end of chain, continue to next bucket
                break;
            }

            int count = *(int*)current;
            for (int i = 0; i < count; i++) {
                string* testString;
                avlTree* testTree;
                char* position = current + sizeof(int) + i * (sizeof(string*) + sizeof(avlTree*));
                memcpy(&testString, position, sizeof(string*));
                memcpy(&testTree, position + sizeof(string*), sizeof(avlTree*));

                int currCount = testTree->getTotalCount(date1, date2);
                std::cout << testString->c_str() << " " << currCount << std::endl;
            }
            char* position = current + bucketSize - sizeof(char*);
            memcpy(&next, position, sizeof(char*));
        } while (next != NULL);
    }
}

void hashtable::displayRecords(std::string disease, std::string date1, std::string date2, std::string country) {
    /*for /diseaseFrequency 
      displays count of all patients with given details for given disease*/

    int bucket = hash(disease);
    char* current;
    char* next = table[bucket]; //get the start of wanted chain
    do {
        current = next; //move forward

        if (current == NULL) { //if reached end of chain, continue
            break;
        }
        int count = *(int*)current;
        for (int i = 0; i < count; i++) {
            string* testString;
            avlTree* testTree;
            char* position = current + sizeof(int) + i * (sizeof(string*) + sizeof(avlTree*));
            memcpy(&testString, position, sizeof(string*));
            memcpy(&testTree, position + sizeof(string*), sizeof(avlTree*)); //copy things from apropriate positions

            if (*testString == disease) { //if the wanted disease was reached print things
                int currCount = testTree->getTotalCount(date1, date2, country);
                std::cout << testString->c_str() << " " << currCount << std::endl;
                return;
            }
        }
        char* position = current + bucketSize - sizeof(char*);
        memcpy(&next, position, sizeof(char*));
    } while (next != NULL);
    // //if wanted disease was not found print a message
    // std::cout << disease << ": 0" << std::endl;
}

void hashtable::displayCurrentRecords(std::string disease) {
    /*for /numCurrentPatients
      displays count of all current patients for the disease, if given*/
    
    int bucket = 0;
    // bool found = true; //flag to use if there is no suck disease
    if (disease != "") {
        // found = false;
        bucket = hash(disease);
    }
    //starting from the bucket the given disease should be in or start if no disease
    for (int j = bucket; j < bucketNumber; ++j) { //for each bucket 
        char* current;
        char* next = table[j]; //get the start of each chain
        do {
            current = next; //move forward

            if (current == NULL) { //if reached end of chain, continue to next loop
                break;
            }
            int count = *(int*)current;
            for (int i = 0; i < count; i++) {
                string* testString;
                avlTree* testTree;
                char* position = current + sizeof(int) + i * (sizeof(string*) + sizeof(avlTree*));
                memcpy(&testString, position, sizeof(string*));
                memcpy(&testTree, position + sizeof(string*), sizeof(avlTree*)); //copy things from current bucket

                //checks and stuff
                if (disease == "" || disease == *testString) {
                    int currCount = testTree->getCurrentCount();
                    std::cout << testString->c_str() << " " << currCount << std::endl;
                    
                    if (disease == *testString) { //if found the given disease exit, if no disease was given continue
                        std::cout << std::endl;
                        return;
                    }
                }
            }
            char* position = current + bucketSize - sizeof(char*);
            memcpy(&next, position, sizeof(char*));
        } while (next != NULL);
    }

    // if (!found) { //unique case of given disease not found
    //     std::cout << disease << ": 0" << std::endl;
    // }
    // std::cout << std::endl;
}

void hashtable::displayTopK_Diseases(std::string k, std::string key, std::string date1, std::string date2) {

    int bucket = hash(key);
    char* current;
    char* next = table[bucket]; //get the start of wanted chain
    do {
        current = next; //move forward

        if (current == NULL) { //if reached end of chain, continue
            break;
        }
        int count = *(int*)current;
        for (int i = 0; i < count; i++) {
            string* testString;
            avlTree* testTree;
            char* position = current + sizeof(int) + i * (sizeof(string*) + sizeof(avlTree*));
            memcpy(&testString, position, sizeof(string*));
            memcpy(&testTree, position + sizeof(string*), sizeof(avlTree*)); //copy things from apropriate positions

            if (*testString == key) { //if the wanted disease was reached
                int t = atoi(k.c_str());
                testTree->printTopK_Diseases(t, date1, date2);
                return;
            }
        }
        char* position = current + bucketSize - sizeof(char*);
        memcpy(&next, position, sizeof(char*));
    } while (next != NULL);
    // //if wanted disease was not found print a message
    // std::cout << key << ": 0" << std::endl;
}

void hashtable::displayTopK_Countries(std::string k, std::string key, std::string date1, std::string date2) {

    int bucket = hash(key);
    char* current;
    char* next = table[bucket]; //get the start of wanted chain
    do {
        current = next; //move forward

        if (current == NULL) { //if reached end of chain, continue
            break;
        }
        int count = *(int*)current;
        for (int i = 0; i < count; i++) {
            string* testString;
            avlTree* testTree;
            char* position = current + sizeof(int) + i * (sizeof(string*) + sizeof(avlTree*));
            memcpy(&testString, position, sizeof(string*));
            memcpy(&testTree, position + sizeof(string*), sizeof(avlTree*)); //copy things from apropriate positions

            if (*testString == key) { //if the wanted disease was reached
                int t = atoi(k.c_str());
                testTree->printTopK_Countries(t, date1, date2);
                return;
            }
        }
        char* position = current + bucketSize - sizeof(char*);
        memcpy(&next, position, sizeof(char*));
    } while (next != NULL);
    // //if wanted disease was not found print a message
    // std::cout << key << ": 0" << std::endl;
}

//~~~~~~~~~~~project 2~~~~~~~~~~~~~

void hashtable::getAgeGroups(string country, string date, string disease, int &a1, int &a2, int &a3, int &a4) {
    //for child log
    
    int bucket = hash(country);
    char* current;
    char* next = table[bucket]; //get the start of wanted chain
    do {
        current = next; //move forward

        if (current == NULL) { //if reached end of chain, continue
            break;
        }
        int count = *(int*)current;
        for (int i = 0; i < count; i++) {
            string* testString;
            avlTree* testTree;
            char* position = current + sizeof(int) + i * (sizeof(string*) + sizeof(avlTree*));
            memcpy(&testString, position, sizeof(string*));
            memcpy(&testTree, position + sizeof(string*), sizeof(avlTree*)); //copy things from apropriate positions

            if (*testString == country) { //if the wanted disease was reached print things
                testTree->getAgeGroups(date, disease, a1, a2, a3, a4);
                return;
            }
        }
        char* position = current + bucketSize - sizeof(char*);
        memcpy(&next, position, sizeof(char*));
    } while (next != NULL);
}

void hashtable::getAgeGroups(string country, string date1, string date2, string disease, int &a1, int &a2, int &a3, int &a4) {
    //for child log
    
    int bucket = hash(country);
    char* current;
    char* next = table[bucket]; //get the start of wanted chain
    do {
        current = next; //move forward

        if (current == NULL) { //if reached end of chain, continue
            break;
        }
        int count = *(int*)current;
        for (int i = 0; i < count; i++) {
            string* testString;
            avlTree* testTree;
            char* position = current + sizeof(int) + i * (sizeof(string*) + sizeof(avlTree*));
            memcpy(&testString, position, sizeof(string*));
            memcpy(&testTree, position + sizeof(string*), sizeof(avlTree*)); //copy things from apropriate positions

            if (*testString == country) { //if the wanted disease was reached print things
                testTree->getAgeGroups(date1, date2, disease, a1, a2, a3, a4);
                return;
            }
        }
        char* position = current + bucketSize - sizeof(char*);
        memcpy(&next, position, sizeof(char*));
    } while (next != NULL);
}

string hashtable::getAllCountries() {
    /*for listCountries */
    string toreturn;
    char* current;
    char* next = table[0]; //get the start of wanted chain
    do {
        current = next; //move forward

        if (current == NULL) { //if reached end of chain, continue
            break;
        }
        int count = *(int*)current;
        for (int i = 0; i < count; i++) {
            string *testString;
            // avlTree *testTree;
            char* position = current + sizeof(int) + i * (sizeof(string*) + sizeof(avlTree*));
            memcpy(&testString, position, sizeof(string*));
            // memcpy(&testTree, position + sizeof(string*), sizeof(avlTree*)); //copy things from apropriate positions

            toreturn += *testString + "\n";
        }
        char* position = current + bucketSize - sizeof(char*);
        memcpy(&next, position, sizeof(char*));
    } while (next != NULL);
    return toreturn;
}

int hashtable::getCount(std::string disease, std::string date1, std::string date2, std::string country) {
    /*for /diseaseFrequency */
    int bucket = hash(disease);
    char* current;
    char* next = table[bucket]; //get the start of wanted chain
    do {
        current = next; //move forward

        if (current == NULL) { //if reached end of chain, continue
            break;
        }
        int count = *(int*)current;
        for (int i = 0; i < count; i++) {
            string* testString;
            avlTree* testTree;
            char* position = current + sizeof(int) + i * (sizeof(string*) + sizeof(avlTree*));
            memcpy(&testString, position, sizeof(string*));
            memcpy(&testTree, position + sizeof(string*), sizeof(avlTree*)); //copy things from apropriate positions

            if (*testString == disease) { //if the wanted disease was reached increase counts accordingly
                int sum = testTree->getTotalCount(date1, date2, country);
                return sum;
            }
        }
        char* position = current + bucketSize - sizeof(char*);
        memcpy(&next, position, sizeof(char*));
    } while (next != NULL);
    return 0;
}

// void hashtable::getCountAges(std::string disease, std::string date1, std::string date2, std::string country,
// int &a1, int &a2, int &a3, int &a4) {
//     /*for /diseaseFrequency 
//       displays count of all patients with given details for given disease*/

//     int bucket = hash(disease);
//     char* current;
//     char* next = table[bucket]; //get the start of wanted chain
//     do {
//         current = next; //move forward

//         if (current == NULL) { //if reached end of chain, continue
//             break;
//         }
//         int count = *(int*)current;
//         for (int i = 0; i < count; i++) {
//             string* testString;
//             avlTree* testTree;
//             char* position = current + sizeof(int) + i * (sizeof(string*) + sizeof(avlTree*));
//             memcpy(&testString, position, sizeof(string*));
//             memcpy(&testTree, position + sizeof(string*), sizeof(avlTree*)); //copy things from apropriate positions

//             if (*testString == disease) { //if the wanted disease was reached increase counts accordingly
//                 testTree->getTotalCountAges(date1, date2, country, a1, a2, a3, a4);
//                 return;
//             }
//         }
//         char* position = current + bucketSize - sizeof(char*);
//         memcpy(&next, position, sizeof(char*));
//     } while (next != NULL);


//     return;
// }

int hashtable::getPatientAdmissions(list<tempClass>& myList, std::string disease, std::string date1, std::string date2, std::string country) {
    /*for /numPatientAdmissions */
    int bucket = hash(disease);
    char* current;
    char* next = table[bucket]; //get the start of wanted chain
    
    do {
        current = next; //move forward

        if (current == NULL) { //if reached end of chain, continue
            break;
        }
        int count = *(int*)current;
        for (int i = 0; i < count; i++) {
            string* testString;
            avlTree* testTree;
            char* position = current + sizeof(int) + i * (sizeof(string*) + sizeof(avlTree*));
            memcpy(&testString, position, sizeof(string*));
            memcpy(&testTree, position + sizeof(string*), sizeof(avlTree*)); //copy things from apropriate positions

            if (*testString == disease) { //if the wanted disease was reached increase counts accordingly
                testTree->countAllCountriesEntry(myList, date1, date2, country);
                break;
            }
        }
        char* position = current + bucketSize - sizeof(char*);
        memcpy(&next, position, sizeof(char*));
    } while (next != NULL);

    return 0;
}

int hashtable::getPatientDischarges(list<tempClass>& myList, std::string disease, std::string date1, std::string date2, std::string country) {
    /*for /getPatientDischarges */
    int bucket = hash(disease);
    char* current;
    char* next = table[bucket]; //get the start of wanted chain
    
    do {
        current = next; //move forward

        if (current == NULL) { //if reached end of chain, continue
            break;
        }
        int count = *(int*)current;
        for (int i = 0; i < count; i++) {
            string* testString;
            avlTree* testTree;
            char* position = current + sizeof(int) + i * (sizeof(string*) + sizeof(avlTree*));
            memcpy(&testString, position, sizeof(string*));
            memcpy(&testTree, position + sizeof(string*), sizeof(avlTree*)); //copy things from apropriate positions

            if (*testString == disease) { //if the wanted disease was reached increase counts accordingly
                testTree->countAllCountriesExit(myList, date1, date2, country);
                break;
            }
        }
        char* position = current + bucketSize - sizeof(char*);
        memcpy(&next, position, sizeof(char*));
    } while (next != NULL);

    return 0;
}
