#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "session.h"

class control_plane {
public:
    explicit control_plane();
    std::shared_ptr<session> create_session(const std::string &imsi);
    void delete_session(const std::string &imsi);
private:
    std::unordered_map<std::string, std::shared_ptr<session>> sessions;
};