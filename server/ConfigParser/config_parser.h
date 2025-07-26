#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "../json.hpp"

using nlohmann::json;

class config_parser {
public:
    config_parser(const std::string& filename = "config.json");
    template<typename T>
    static T get(const std::string& key);
    bool has(const std::string& key) const;
private:
    json data;
};