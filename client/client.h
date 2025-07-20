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
#include <random>
#include <memory>
#include <algorithm>

class UDPClient {
public:
    
    UDPClient(const std::string& server_ip, int server_port);

    ~UDPClient();

    void send_message(const std::string& message);

    std::string receive_response();

private:
    static const int MAX_EVENTS = 10;
    static const int BUF_SIZE = 1024;
    
    std::string server_ip;
    int server_port;
    int client_fd;
    sockaddr_in server_addr;

    void create_socket();

    void setup_server_address();
};