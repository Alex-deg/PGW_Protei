#include "server.h"

UDPServer::UDPServer(int port){
    this->port = port;
    epoll_fd = -1;
    server_fd = -1;
    std::ifstream json_file_stream("../example.json");
    json data = json::parse(json_file_stream);
    std::cout << "server_ip = " << data["server_ip"].get<std::string>() << std::endl;
    std::cout << "server_port = " << data["server_port"].get<unsigned int>() << std::endl;
    std::cout << "log_file = " << data["log_file"].get<std::string>() << std::endl;
    std::cout << "log_level = " << data["log_level"].get<std::string>() << std::endl;
    createSocket();
    setupEpoll();
}

UDPServer::~UDPServer() {
    if (server_fd != -1) close(server_fd);
    if (epoll_fd != -1) close(epoll_fd);
}

void UDPServer::run() {
    std::cout << "UDP server running on port " << port << "..." << std::endl;
    
    struct epoll_event events[MAX_EVENTS];

    while (true) {
        int events_cnt = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (events_cnt == -1) {
            throw std::runtime_error("epoll_wait failed");
        }
        for (int i = 0; i < events_cnt; ++i) {
            if (events[i].data.fd == server_fd) {
                handleClientData(events[i].data.fd);
            }
        }
    }
}

void UDPServer::createSocket() {
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1) {
        throw std::runtime_error("Не удалось создать сокет");
    }

    // ???
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        throw std::runtime_error("setsockopt failed");
    }

    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address))) {
        close(server_fd);
        throw std::runtime_error("bind failed");
    }

    set_nonblocking(server_fd);

}

void UDPServer::setupEpoll() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        throw std::runtime_error("epoll_create1 failed");
    }

    struct epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        throw std::runtime_error("epoll_ctl: server_fd failed");
    }
}

void UDPServer::handleClientData(int fd) {

    char buffer[BUF_SIZE];
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (true) {
        ssize_t bytes_received = recvfrom(server_fd, buffer, BUF_SIZE, 0,
                                        (struct sockaddr*)&client_addr, &client_len);
        
        if (bytes_received == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // Нет данных для чтения
                break;
            } else {
                throw std::runtime_error("Failed to receive data");
            }
        }

        std::string message(buffer, bytes_received);
        std::string response = "Echo: ";

        std::time_t currentTime = std::time(nullptr);
        std::tm localTime = *std::localtime(&currentTime);
        std::string now = std::ctime(&currentTime);
        now.pop_back();
        now += ","+message+",";
        // Обработка полученных данных
        if(imsi.setIMSI(message)){

            cdr.writeLine(now+"connection is successful");

            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
            
            std::cout << "Received from " << client_ip << ":" << ntohs(client_addr.sin_port)
                        << " - " << message << std::endl;
            response += message;

        }
        else{
            cdr.writeLine(now+"connection is wrong");
            response += "Your IMSI isn't correct";
        }
        
        sendto(server_fd, response.c_str(), response.size(), 0,
                (struct sockaddr*)&client_addr, client_len);
    }
}

int UDPServer::set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if(flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}