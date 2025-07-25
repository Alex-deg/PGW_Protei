#pragma once

#include <memory>
#include <string>
#include <chrono>

class session {
public:
    static std::shared_ptr<session> create(std::string imsi);
    std::chrono::steady_clock::time_point getCreatedTime();
private:
    session(std::string imsi);
    std::string imsi;
    std::chrono::steady_clock::time_point created_at; 
};