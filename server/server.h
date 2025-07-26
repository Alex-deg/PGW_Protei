#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <cerrno>
#include <stdexcept>
#include <memory>
#include <fcntl.h>
#include <string>
#include <ctime>

#include "PGW/src/data_plane.h"
#include "PGW/src/control_plane.h"
#include "FileHandler/FileHandler.h"
#include "../json.hpp"
#include "../BCD/bcd.h"
#include "../httplib.h"
#include "Logger/logger.h"
#include "ConfigParser/config_parser.h"

using nlohmann::json;

// СДЕЛАТЬ НЕ ЛОКАЛЬНЫЕ ПУТИ

class UDPServer {
public:

    UDPServer(std::shared_ptr<config_parser> config);
    void run();
    ~UDPServer();

    // Геттеры
    std::unordered_map<std::string, std::shared_ptr<session>>* get_cp_sessions();
    std::shared_ptr<FileHandler> get_cdr_journal();
    
private:

    // Values
    // Constants
    static const int MAX_EVENTS = 10;
    static const int BUF_SIZE = 1024;
    // Config settings
    std::shared_ptr<config_parser> config;
    // Aux values
    int epoll_fd;
    int server_fd;
    std::shared_ptr<control_plane> cp;
    std::shared_ptr<data_plane> dp;
    std::shared_ptr<FileHandler> cdr;

    // Methods
    void createSocket();
    void setupEpoll();
    void handleClientData(int fd);
    int set_nonblocking(int fd);  

};