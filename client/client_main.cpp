#include "client.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>" << std::endl;
        return 1;
    }

    try {
        std::string server_ip = argv[1];
        int server_port = std::stoi(argv[2]);

        UDPClient client(server_ip, server_port);

        while (true) { // Добавить IMSI (15 цифр)
            std::cout << "Enter message (or 'quit' to exit): ";
            std::string message;
            std::getline(std::cin, message);

            if (message == "quit") {
                break;
            }

            client.send_message(message);
            
            try {
                std::string response = client.receive_response();
                std::cout << "Received: " << response << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}