#include "session.h"

std::shared_ptr<session> session::create(std::string imsi) {
    return std::shared_ptr<session>(new session(std::move(imsi)));
}

std::chrono::steady_clock::time_point session::getCreatedTime()
{
    return created_at;
}

session::session(std::string imsi){
    this->imsi = std::move(imsi);
    this->created_at = std::chrono::steady_clock::now();
}