#pragma once

#include <string>
#include "../json.hpp"

using nlohmann::json;

class config_parser{

public:
    config_parser(const std::string &path);
private:
    template<typename T>
    T config_parser::extract_value(const nlohmann::json &json, std::string_view key);

};