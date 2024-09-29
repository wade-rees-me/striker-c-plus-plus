#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
#include "utilities.hpp"

std::string Utilities::addCommas(int64_t number) {
    std::string str = std::to_string(number);
    int insertPosition = str.length() - 3;
    
    while (insertPosition > 0) {
        str.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    
    return str;
}

