#pragma once

#include <cstdint>
#include <expected>
#include <string>
#include <vector>

#include "control_plane.h"

class udp_server;

class data_plane {
public:
    explicit data_plane(std::shared_ptr<control_plane> cp);
    std::string handle_packet(const std::string &imsi);
private:
    friend udp_server;
    std::shared_ptr<control_plane> cp;
};