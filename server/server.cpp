#include "server.h"

UDPServer::UDPServer(std::unordered_map<std::string, std::shared_ptr<session>>*& sessions){
    parse_server_config();
    cdr = std::make_shared<FileHandler>(cdr_file);
    cp = std::make_shared<control_plane>(black_list);
    dp = std::make_shared<data_plane>(cp);
    sessions = cp->get_sessions();
    std::cout << "server.cpp: " << sessions << std::endl;
    epoll_fd = -1;
    server_fd = -1;
    createSocket();
    setupEpoll();
}

UDPServer::~UDPServer() {
    if (server_fd != -1) close(server_fd);
    if (epoll_fd != -1) close(epoll_fd);
}

void UDPServer::run() {
    std::cout << "UDP server running on server_port " << server_port << "..." << std::endl;
    
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
    address.sin_port = htons(server_port);

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

    std::vector<uint8_t> buffer(BUF_SIZE);
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (true) {
        ssize_t bytes_received = recvfrom(server_fd, buffer.data(), BUF_SIZE, 0,
                                        (struct sockaddr*)&client_addr, &client_len);
        
        if (bytes_received == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // Нет данных для чтения
                break;
            } else {
                throw std::runtime_error("Failed to receive data");
            }
        }

        buffer.resize(bytes_received);

        std::string message = BCD::decode_imsi(buffer);

        std::time_t currentTime = std::time(nullptr);
        std::tm localTime = *std::localtime(&currentTime);
        std::string now = std::ctime(&currentTime);
        now.pop_back();
        now += ","+message+",";

        // Обработка полученных данных

        auto result = dp->handle_packet(message);

        if(result.first){
            cdr->writeLine(now+result.second);

            // char client_ip[INET_ADDRSTRLEN];
            // inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
            
            // std::cout << "Received from " << client_ip << ":" << ntohs(client_addr.sin_port)
            //             << " - " << message << std::endl;
            // response += message;

        }
        else{
            cdr->writeLine(now+result.second);
        }
        
        // Разобраться с типом буфера
        
        sendto(server_fd, result.second.c_str(), result.second.size(), 0,
                (struct sockaddr*)&client_addr, client_len);
    }
}

int UDPServer::set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if(flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void UDPServer::parse_server_config(){
    std::ifstream json_file_stream("../server_config.json");
    json data = json::parse(json_file_stream);
    udp_ip = data["udp_ip"].get<std::string>();
    server_port = data["udp_port"].get<unsigned int>();
    session_timeout_sec = data["session_timeout_sec"].get<unsigned int>();
    cdr_file = data["cdr_file"].get<std::string>();
    http_port = data["http_port"].get<unsigned int>();
    graceful_shutdown_rate = data["graceful_shutdown_rate"].get<unsigned int>();
    log_file = data["log_file"].get<std::string>();
    log_level = data["log_level"].get<std::string>();
    black_list = data["blacklist"].get<std::string>();
}