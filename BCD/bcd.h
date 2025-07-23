#pragma once

#include <string>

class BSD{
public:
    static std::string encode_bcd(const std::string &data);
    static std::string decode_bcd(const std::string &code);
};