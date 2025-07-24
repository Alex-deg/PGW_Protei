#include "server.h"

// Возможно лучше здесь распарсить конфиг и просто передавать настройки в конструктор 


int main(int argc, char* argv[]) {
    try {
        system("bash ../scripts/generate_blacklist.sh");
        UDPServer server;
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}