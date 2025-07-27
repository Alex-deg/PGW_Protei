#include "client.h"

#include "../libs/BCD/bcd.h"

int main(int argc, char* argv[]) {
    try {

        UDPClient client;

        while (true) { // Добавить IMSI (15 цифр)
            std::cout << "Enter IMSI (or 'quit' to exit): ";
            std::string message;
            std::getline(std::cin, message);

            if (message == "quit") {
                break;
            }

            client.send_message(BCD::encode_imsi(message));
            
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