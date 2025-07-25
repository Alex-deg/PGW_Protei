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
#include <stack>
#include <string>
#include <ctime>

#include "PGW/src/IMSI_handler.h"
#include "PGW/src/data_plane.h"
#include "PGW/src/control_plane.h"
#include "FileHandler/FileHandler.h"
#include "../json.hpp"
#include "../BCD/bcd.h"
#include "../httplib.h"
#include "Logger/logger.h"

using nlohmann::json;


// СДЕЛАТЬ НЕ ЛОКАЛЬНЫЕ ПУТИ

class UDPServer {
public:

    UDPServer(std::unordered_map<std::string, std::shared_ptr<session>>*& sessions);
    void run();
    ~UDPServer();
    
private:

    static const int MAX_EVENTS = 10;
    static const int BUF_SIZE = 1024;
    
    std::string udp_ip;
    int server_port;
    int epoll_fd;
    int server_fd;

    std::string log_file;
    std::string log_level;

    std::string black_list;

    int session_timeout_sec;
    std::string cdr_file;
    int http_port;
    int graceful_shutdown_rate;

    std::shared_ptr<control_plane> cp;
    std::shared_ptr<data_plane> dp;

    IMSI imsi;
    std::shared_ptr<FileHandler> cdr;
    //std::vector<struct epoll_event> events_{MAX_EVENTS};

    void createSocket();
    void setupEpoll();
    void handleClientData(int fd);

    void parse_server_config();

    int set_nonblocking(int fd);

    std::string client_request;

};