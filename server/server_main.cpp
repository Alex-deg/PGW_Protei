#include "server.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Слишком мало аргументов: должно быть 2" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        int port = std::stoi(argv[1]);
        UDPServer server(port);
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}