#include "control_plane.h"

control_plane::control_plane(){}

std::shared_ptr<session> control_plane::create_session(const std::string &imsi) {
    for(auto &x : sessions){
        if(x.first == imsi)
            return nullptr;
    }
    sessions[imsi] = session::create(imsi);
    return sessions[imsi];
}

void control_plane::delete_session(const std::string &imsi) { sessions.erase(imsi); }