#ifndef JASON_HPP
#define JASON_HPP

#include <iostream>
#include <string>
#include <fstream>

#include "list.hpp"
#include "utilities.hpp"
// #include <avl.hpp>

class data {
protected:
    std::string key;
public:
    data(std::string nkey);
    data();
    virtual ~data();

    std::string getKey();
    void setKey(std::string nkey);
    virtual void addValue(std::string value) {};
    virtual void print(int tabs) {};
    virtual std::string stringy() {return std::string("lol"); };
};
//~~~~~~~~~~~~
class property: public data {
private:
    std::string value;
public:
    property(std::string nkey, std::string nvalue);
    property(std::string nvalue);
    ~property();

    std::string getValue();
    void addValue(std::string nvalue);
    virtual void print(int tabs);
    virtual std::string stringy();
};
//~~~~~~~~~~~~
class array: public data {
private:
    list<property> content;
public:
    array(std::string nkey);
    ~array();

    list<property>* getContent();
    void addValue(std::string nvalue);
    virtual void print(int tabs);
    virtual std::string stringy();
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class jsonObject {
private:
public:
    list<data> content;
    jsonObject();
    ~jsonObject();

    void addaray(std::string key);
    void insert(std::string key, std::string value);
    void addProperty(std::string key, std::string value);
    void print();
    std::string stringy();
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class jsonParser {
private:
    
    /*************FLAGS******************/
    bool item_start_flag = false;
    bool item_end_flag = false;
    bool value_start_flag = false;
    bool value_end_flag = false;
    bool escape_char_flag = false;
    int list_counter = 0;

    /*************ENUMS******************/
    enum char_code {
        item_value,
        escape,
        comma,
        list_start,
        list_end,
        nada
    };

    char_code enum_it (char const& inString) {
        if (inString == '\"') return item_value;
        if (inString == '\\') return escape;
        if (inString == ',') return comma;
        if (inString == '[') return list_start;
        if (inString == ']') return list_end;
        return nada;
    }

    jsonObject *object;
public:
    jsonParser();
    ~jsonParser();

    jsonObject* parse(std::string path_to_file);
};



#endif /*JASON_HPP*/
