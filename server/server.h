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

#include "../PGW/src/IMSI_handler.h"
#include "../FileHandler/FileHandler.h"

class UDPServer {
public:

    UDPServer(int port);
    void run();
    ~UDPServer();
    
private:

    static const int MAX_EVENTS = 10;
    static const int BUF_SIZE = 1024;
    
    int port;
    int epoll_fd;
    int server_fd;

    IMSI imsi;
    FileHandler cdr;
    //std::vector<struct epoll_event> events_{MAX_EVENTS};

    void createSocket();
    void setupEpoll();
    void handleClientData(int fd);

    int set_nonblocking(int fd);

    std::string client_request;

};