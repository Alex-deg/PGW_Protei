#include "server.h"

UDPServer::UDPServer(std::unordered_map<std::string, std::shared_ptr<session>>*& sessions,
                     config_parser &config){
    this->config = config;
    Logger::init(config.get<std::string>("log_file"));
    Logger::set_level(config.get<std::string>("log_level"));
    cdr = std::make_shared<FileHandler>(config.get<std::string>("cdr_file"));
    cp = std::make_shared<control_plane>(config.get<std::string>("blacklist"));
    dp = std::make_shared<data_plane>(cp);
    sessions = cp->get_sessions();

    epoll_fd = -1;
    server_fd = -1;
    createSocket();
    setupEpoll();
}

UDPServer::~UDPServer() {
    if (server_fd != -1) close(server_fd);
    if (epoll_fd != -1) close(epoll_fd);
    Logger::info("Завершение работы сервера");
}

void UDPServer::run() {
    std::cout << "UDP server running on server_port " << config.get<int>("udp_port") << "..." << std::endl;
    Logger::info("Сервер запущен");
    struct epoll_event events[MAX_EVENTS];

    while (true) {
        int events_cnt = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (events_cnt == -1) {
            Logger::error("epoll_wait failed");
            throw std::runtime_error("epoll_wait failed");
        }
        Logger::info("Перебор всех доступных событий");
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
        Logger::error("Не удалось создать сокет");
        throw std::runtime_error("Не удалось создать сокет");
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        Logger::error("setsockopt failed");
        throw std::runtime_error("setsockopt failed");
    }

    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(server_port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address))) {
        close(server_fd);
        Logger::error("bind failed");
        throw std::runtime_error("bind failed");
    }

    set_nonblocking(server_fd);
    Logger::info("Создание сокета");
}

void UDPServer::setupEpoll() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        Logger::error("epoll_create1 failed");
        throw std::runtime_error("epoll_create1 failed");
    }

    struct epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        Logger::error("epoll_ctl: server_fd failed");
        throw std::runtime_error("epoll_ctl: server_fd failed");
    }
    Logger::info("Инициализация epoll");
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
                Logger::warn("Нет данных для чтения");
                break;
            } else {
                Logger::error("Failed to receive data");
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

        auto result = dp->handle_packet(message);

        cdr->writeLine(now+result.second);
        
        sendto(server_fd, result.second.c_str(), result.second.size(), 0,
                (struct sockaddr*)&client_addr, client_len);
        Logger::info("Отправка данных клиенту");
    }
    Logger::info("Обработка клиентских данных");
}

int UDPServer::set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if(flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}