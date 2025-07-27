#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "session.h"
#include "../../../libs/FileHandler/FileHandler.h"
#include "../../../libs/Logger/logger.h"

class control_plane {
public:
    explicit control_plane(const std::string &path_to_blacklist);
    std::pair<std::shared_ptr<session>, std::string> create_session(const std::string &imsi);
    void delete_session(const std::string &imsi);
    std::unordered_map<std::string, std::shared_ptr<session>>* get_sessions();
private:
    std::vector<std::string> IMSI_blacklist;
    std::unordered_map<std::string, std::shared_ptr<session>> sessions;
};