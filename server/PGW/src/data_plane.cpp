#include <string>
#include <thread>

#include "data_plane.h"

// стоит ли хранить сессии в массиве?

std::string data_plane::handle_packet(const Packet &packet) {
    std::string imsi = "123123123123123";/*utility::parse_imsi_from_bcd(packet)*/ // сделать шифровку imsi

    std::shared_ptr result = _control_plane.create_session(imsi);
    if (not result) {
        return "rejected";
    }

    return "created";
}

data_plane::data_plane(control_plane &control_plane){
    this->_control_plane = control_plane;
}
