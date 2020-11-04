#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

class hashtable {
private:
    int bucketNumber;
    int bucketSize;
    char** table;
    int recordsPerBucket;
    std::string record::*key;//pointer to data member of record, which is a string

    unsigned int hash(record* rec);
    unsigned int hash(std::string temp);
    bool find(record* rec, char* &response);
    char* newBucket();
public:
    hashtable(int buckets, int bucketSize, bool isCountryKey);
    ~hashtable();

    int getRecordsPerBucket();

    void insert(record* rec);
    void displayRecords(std::string date1, std::string date2);
    void displayRecords(std::string disease, std::string date1, std::string date2, std::string country);
    void displayCurrentRecords(std::string disease);
    void displayTopK_Diseases(std::string k, std::string key, std::string date1, std::string date2);
    void displayTopK_Countries(std::string k, std::string key, std::string date1, std::string date2);

    //project 2
    void getAgeGroups(std::string country, std::string date, std::string disease, int &a1, int &a2, int &a3, int &a4);
    void getAgeGroups(std::string country, std::string date1, std::string date2, std::string disease, int &a1, int &a2, int &a3, int &a4);
    std::string getAllCountries();
    int getCount(std::string disease, std::string date1, std::string date2, std::string country);
    // void getCountAges(std::string disease, std::string date1, std::string date2, std::string country, int &a1, int &a2, int &a3, int &a4);
    int getPatientAdmissions(list<tempClass>& myList, std::string disease, std::string date1, std::string date2, std::string country);
    int getPatientDischarges(list<tempClass>& myList, std::string disease, std::string date1, std::string date2, std::string country);
};

#endif /* HASHTABLE_HPP */