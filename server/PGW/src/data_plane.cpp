#include <string>
#include <thread>

#include "data_plane.h"

// стоит ли хранить сессии в массиве?

std::string data_plane::handle_packet(const std::string &imsi) {
    std::shared_ptr result = cp->create_session(imsi);
    if (!result) {
        return "rejected";
    }
    return "created";
}

data_plane::data_plane(std::shared_ptr<control_plane> cp){
    this->cp = cp;
}
