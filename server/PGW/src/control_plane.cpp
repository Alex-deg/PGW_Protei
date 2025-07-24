#include "control_plane.h"

control_plane::control_plane(const std::string &path_to_blacklist){
    std::shared_ptr<FileHandler> IMSI_bl = std::make_shared<FileHandler>(path_to_blacklist);
    std::string cur_imsi;
    while(true){
        cur_imsi = IMSI_bl->readLine();
        if(cur_imsi != ""){
            IMSI_blacklist.push_back(cur_imsi);
        }else break;
    }
}

std::pair<std::shared_ptr<session>, std::string> control_plane::create_session(const std::string &imsi) {
    
    if(imsi.size() != 15)
        return {nullptr, "длина IMSI не соответствует стандарту 3GPP!"}; 
    
    for(auto &x : sessions){
        if(x.first == imsi)
            return { nullptr, "Сессия с таким IMSI уже существует!" };
    }

    for (auto &x : IMSI_blacklist){
        if(x == imsi)
            return { nullptr, "IMSI в черном списке!" };
    }

    for(auto &x : imsi){
        if (!(x > 47 && x < 58))
            return { nullptr, "IMSI содержит символы отличные от цифр!" };
    }

    sessions[imsi] = session::create(imsi);
    return { sessions[imsi], "IMSI = " + imsi };
}

void control_plane::delete_session(const std::string &imsi) { sessions.erase(imsi); }

std::unordered_map<std::string, std::shared_ptr<session>> control_plane::get_sessions()
{
    return sessions;
}
