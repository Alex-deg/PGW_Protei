#include "config_parser.h"

config_parser::config_parser(const std::string &path){

    
    
}

template<typename T>
T config_parser::extract_value(const nlohmann::json &json, std::string_view key) {
    if (json.contains(key) && !json[key].is_null()) {
        return json[key].get<T>();
    }
    return nullptr;
}