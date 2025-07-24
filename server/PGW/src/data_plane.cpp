#include <string>
#include <thread>

#include "data_plane.h"

// стоит ли хранить сессии в массиве?

std::pair<bool, std::string> data_plane::handle_packet(const std::string &imsi) {
    auto res = cp->create_session(imsi);
    if (!res.first) {
        return {false, "rejected: " + res.second};
    }
    return {true, "created: " + res.second};
}

data_plane::data_plane(std::shared_ptr<control_plane> cp){
    this->cp = cp;
}
