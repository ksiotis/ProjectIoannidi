#include <iostream>
#include <string>
#include <fstream>

enum char_code {
    item_value,
    escape,
    comma,
    file_start,
    file_end,
    list_start,
    list_end
};


char_code enum_it (char const& inString) {
    if (inString == '\"') return item_value;
    if (inString == '\\') return escape;
    if (inString == ',') return comma;
    if (inString == '[') return list_start;
    if (inString == ']') return list_end;
}



int main () {

    // char str[256];

    std::cout << "Datasets/X/www.camerafarm.com.au/705.json";
//   std::cin.get (str,256);    // get c-string

    std::ifstream is("Datasets/X/www.camerafarm.com.au/705.json");     // open file
    std::string word ="";

    /*************FLAGS******************/
    bool item_start_flag = false;
    bool item_end_flag = false;
    bool value_start_flag = false;
    bool value_end_flag = false;
    bool escape_char_flag = false;
    int list_counter = 0;


    char c;
    while (is.get(c)){          // loop getting single characters
        switch(enum_it(c)){
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
                    std::cout << "KEY IS " << word << " : "; /* KEY */
                    word = "";
                    item_end_flag = true;
                }else if(!value_start_flag){
                    value_start_flag = true;
                    word="";
                }else{
                    value_end_flag = true;
                    std::cout << "\"" << word << "\" "; /* VALUE */
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
                        std::cout << ",";
                        word="";
                    }else{
                        std::cout << std::endl;       /* END OF OBJECT (KEY,VALUES)*/
                        item_start_flag = false;
                        item_end_flag = false;
                        value_start_flag = false;
                        value_end_flag = false;
                        word="";
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
    std::cout << std::endl;
    is.close();                // close file

    return 0;
}