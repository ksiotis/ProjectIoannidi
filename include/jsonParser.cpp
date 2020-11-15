#include "jsonParser.hpp"

//~~~~~~~~~~~~~~prop~~~~~~~~~~~~~~~~~

data::data(std::string nkey):
    key(nkey)
{}

data::data():
    key(std::string(""))
{}

data::~data() {}

std::string data::getKey() { return key; }

void data::setKey(std::string nkey) { key = nkey; }

//~~~~~~~~~~~~~~property~~~~~~~~~~~~~~~~~

property::property(std::string nkey, std::string nvalue):
    data(nkey),
    value(nvalue)
{}

property::property(std::string nvalue):
    value(nvalue)
{}

property::~property() {}

std::string property::getValue() { return value; }

void property::addValue(std::string nvalue) {
    value = nvalue;
}

//~~~~~~~~~~~~~~array~~~~~~~~~~~~~~~~~

array::array(std::string nkey):
    data(nkey)
{}

array::~array() {
    content.emptyList(true);
}

list<property>* array::getContent() { return &content; }

void array::addValue(std::string nvalue) {
    property *temp = new property(nvalue);
    content.insert(temp);
}

//~~~~~~~~~~~~~~jsonObject~~~~~~~~~~~~~~~~~

jsonObject::jsonObject() {}

jsonObject::~jsonObject() {
    content.emptyList(true);
}

void jsonObject::addaray(std::string key) {
    array* temp = new array(key);
    content.insert(temp);
}

void jsonObject::insert(std::string key, std::string value) {
    listNode<data>* current = content.getStart();
    while (current != NULL) {

        if (current->getContent()->getKey() == key) { //TODO check if actually array?
            current->getContent()->addValue(value);
            return;
        }
        current = current->getNext();
    }
    //if wanted name is not inside, add it and add value
    addaray(key);
    content.getEnd()->getContent()->addValue(value);
}

void jsonObject::addProperty(std::string key, std::string value) {
    property* temp = new property(key, value);
    content.insert(temp);
}

//~~~~~~~~~~~~~~jsonParser~~~~~~~~~~~~~~~~~

jsonParser::jsonParser() {}

jsonParser::~jsonParser() {}

void jsonParser::parse(std::string path_to_file) {
    try {
        std::ifstream is(path_to_file); // open file

        if (!is.is_open()) //check if file is open
            throw "Unable to open file!";

        std::string word ="", key, value;
        char c;
        while (is.get(c)) { // loop getting single characters
            switch(enum_it(c)) {
                case item_value:
                    if(escape_char_flag){
                        escape_char_flag = false;
                        word+=c;
                        break;
                    }
                    if(!item_start_flag){
                        word = "";
                        item_start_flag = true;
                    }else if(!item_end_flag){
                        // std::cout << "KEY IS " << word << " : "; /* KEY */
                        key = word;

                        word = "";
                        item_end_flag = true;
                    }else if(!value_start_flag){
                        value_start_flag = true;
                        word="";
                    }else{
                        value_end_flag = true;

                        // std::cout << "\"" << word << "\" "; /* VALUE */
                        value = word;
                        if (list_counter == 0) {
                            object.addProperty(key, value);
                        }
                        else {
                            object.insert(key, value);
                        }

                        word = "";
                    }
                    escape_char_flag = false;
                    break;
                case escape:
                    escape_char_flag = true;
                    word+=c;
                    break;
                case comma:
                    if( (item_start_flag && !item_end_flag) || (value_start_flag && !value_end_flag) ){
                        word+=c;
                        break;
                    }
                    if(item_start_flag && item_end_flag){
                        if(list_counter > 0 ){
                            value_start_flag = false;
                            value_end_flag = false;
                            // std::cout << ",";
                            word="";
                        }else{
                            // std::cout << std::endl;       /* END OF OBJECT (KEY,VALUES)*/
                            item_start_flag = false;
                            item_end_flag = false;
                            value_start_flag = false;
                            value_end_flag = false;
                            word="";
                            key="";
                            value="";
                            list_counter=0;
                        }
                    }
                    escape_char_flag = false;
                    break;
                case list_start:
                    if( (item_start_flag && !item_end_flag) || (value_start_flag && !value_end_flag) ){
                        word+=c;
                        break;
                    }
                    list_counter++;
                    escape_char_flag = false;
                    break;
                case list_end:
                    if( (item_start_flag && !item_end_flag) || (value_start_flag && !value_end_flag) ){
                        word+=c;
                        break;
                    }
                    if(list_counter>0)
                        list_counter--;
                    escape_char_flag = false;
                    break;
                default:
                    escape_char_flag = false;
                    word+=c;
                    break;
            }
        }
        // std::cout << std::endl;
        is.close();                // close file
    }
    catch(const char *e) {
        std::cerr << e << '\n';
    }
}
