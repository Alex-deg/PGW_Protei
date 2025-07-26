#include "client.h"

UDPClient::UDPClient() 
    : client_fd(-1) {
    parse_client_config();
    create_socket();
    setup_server_address();
    
}

UDPClient::~UDPClient() {
    if (client_fd != -1) {
        close(client_fd);
    }
}

void UDPClient::send_message(std::vector<uint8_t> data) {
    ssize_t bytes_sent = sendto(client_fd, data.data(), data.size(), 0,
                                (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (bytes_sent == -1) {
        throw std::runtime_error("Failed to send message");
    }
}

std::string UDPClient::receive_response() {
    int timeout_sec = 2;
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(client_fd, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = timeout_sec;
    timeout.tv_usec = 0;

    int ready = select(client_fd + 1, &read_fds, nullptr, nullptr, &timeout);
    if (ready == -1) {
        throw std::runtime_error("select() failed");
    } else if (ready == 0) {
        throw std::runtime_error("Timeout waiting for response");
    }

    char buffer[1024];
    sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);

    ssize_t bytes_received = recvfrom(client_fd, buffer, sizeof(buffer), 0,
                                    (struct sockaddr*)&from_addr, &from_len);
    if (bytes_received == -1) {
        throw std::runtime_error("Failed to receive response");
    }

    // Проверяем, что ответ пришел с ожидаемого сервера
    if (from_addr.sin_addr.s_addr != server_addr.sin_addr.s_addr ||
        from_addr.sin_port != server_addr.sin_port) {
        throw std::runtime_error("Received response from unexpected source");
    }

    return std::string(buffer, bytes_received);
}
    
void UDPClient::create_socket() {
    client_fd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
    if (client_fd == -1) {
        throw std::runtime_error("Failed to create socket");
    }
}

void UDPClient::setup_server_address() {
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0) {
        close(client_fd);
        throw std::runtime_error("Invalid server address");
    }
}

void UDPClient::parse_client_config(){
    std::ifstream json_file_stream("../client_config.json");
    json data = json::parse(json_file_stream);
    server_ip = data["server_ip"].get<std::string>();
    server_port = data["server_port"].get<unsigned int>();
    log_file = data["log_file"].get<std::string>();
    log_level = data["log_level"].get<std::string>();
}
