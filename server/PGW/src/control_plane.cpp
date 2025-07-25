#include "control_plane.h"

control_plane::control_plane(const std::string &path_to_blacklist){
    std::shared_ptr<FileHandler> IMSI_bl = std::make_shared<FileHandler>(path_to_blacklist);
    std::string cur_imsi;
    while(true){
        cur_imsi = IMSI_bl->readLine();
        if(cur_imsi != ""){
            IMSI_blacklist.push_back(cur_imsi);
        } else break;
    }
    Logger::info(Logger::time_now_to_string() + ": " + "Инициализация черного списка IMSI");
    Logger::info(Logger::time_now_to_string() + ": " + "Создание control plane");
}

std::pair<std::shared_ptr<session>, std::string> control_plane::create_session(const std::string &imsi) {
    
    if(imsi.size() != 15){
        Logger::warn(Logger::time_now_to_string() + ": " + "длина IMSI не соответствует стандарту 3GPP!");
        return {nullptr, "длина IMSI не соответствует стандарту 3GPP!"}; 
    }
    
    for(auto &x : sessions){
        if(x.first == imsi){
            Logger::warn(Logger::time_now_to_string() + ": " + "Сессия с таким IMSI уже существует!");
            return { nullptr, "Сессия с таким IMSI уже существует!" };
        }
    }

    for (auto &x : IMSI_blacklist){
        if(x == imsi){
            Logger::warn(Logger::time_now_to_string() + ": " + "IMSI в черном списке!");
            return { nullptr, "IMSI в черном списке!" };
        }
    }

    for(auto &x : imsi){
        if (!(x > 47 && x < 58)){
            Logger::warn(Logger::time_now_to_string() + ": " + "IMSI содержит символы отличные от цифр!");
            return { nullptr, "IMSI содержит символы отличные от цифр!" };
        }
    }

    sessions[imsi] = session::create(imsi);
    Logger::info(Logger::time_now_to_string() + ": " + "Сессия успешно создана!");
    return { sessions[imsi], "IMSI = " + imsi };
}

void control_plane::delete_session(const std::string &imsi) { sessions.erase(imsi); }

std::unordered_map<std::string, std::shared_ptr<session>>* control_plane::get_sessions()
{
    return &sessions;
}
