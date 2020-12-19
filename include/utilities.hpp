#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <string>

int max(int first, int second);
std::string id_to_path(std::string id,std::string folder);
std::string removeWord(std::string str, std::string word);

class unit_testing {
    public:
        std::string id;

    unit_testing(std::string x);
    ~unit_testing();
};

class generic{
    protected:
        std::string id;
    public:
        generic(std::string id): id(id){};
        virtual ~generic() {};
        std::string getId() { return id; }
};

#endif /* UTILITIES_HPP */