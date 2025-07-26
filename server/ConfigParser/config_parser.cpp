#include "config_parser.h"

config_parser::config_parser(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open config file: " + filename);
    }
    file >> data;
}

template<typename T>
T config_parser::get(const std::string& key){
    return data[key].get<T>();
}

bool config_parser::has(const std::string& key) const {
    return data.contains(key);
}