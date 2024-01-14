#include "parser.hpp"

namespace Parser {
    int8_t get_int_from_hex_char(const char hexChar) {
        char upperHexChar = std::toupper(hexChar);
        auto mapPair = hexMap.find(upperHexChar);

        if (mapPair != hexMap.end()) {
            return mapPair->second;
        }

        return -1;
    }

    bool is_hex_char(const char toCheck) {
        char upper = std::toupper(toCheck);
        auto mapPair = hexMap.find(upper);

        return mapPair != hexMap.end() ? true : false;
    }
}