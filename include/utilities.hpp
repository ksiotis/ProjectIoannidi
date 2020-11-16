#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <string>

int max(int first, int second);

class unit_testing {
    public:
        std::string id;

    unit_testing(std::string x);
    ~unit_testing();
};

#endif /* UTILITIES_HPP */