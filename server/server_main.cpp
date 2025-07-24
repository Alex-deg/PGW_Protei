#include "server.h"

#include <thread>

// Возможно лучше здесь распарсить конфиг и просто передавать настройки в конструктор 

const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<session>>> sessions;

void run_http_server() {

    httplib::Server http_server;

    http_server.Get("/check_subscriber", [](const httplib::Request& req, httplib::Response& res) {
        auto jmsi = req.get_param_value("jmsi");
        if (sessions->find(jmsi) != sessions->end()) {
            res.set_content("active", "text/plain"); 
        } else {
            res.status = 404;
            res.set_content("Session not found", "text/plain");
        }
    });

    // Эндпоинт /stop
    // server.Post("/stop", [](const httplib::Request& req, httplib::Response& res) {
    //     if (is_shutting_down) {
    //         res.status = 400;
    //         res.set_content("Shutdown already in progress", "text/plain");
    //         return;
    //     }

    //     int speed = 1; // Скорость по умолчанию
    //     if (req.has_param("speed")) {
    //         speed = std::stoi(req.get_param_value("speed"));
    //     }

    //     std::thread(graceful_shutdown, speed).detach();
    //     res.set_content("Graceful shutdown started", "text/plain");
    // });

    std::cout << "HTTP Server started on port 8080\n";
    http_server.listen("0.0.0.0", 8080);
}

void run_udp_server(){
    UDPServer server(sessions);
    server.run();
}

int main(int argc, char* argv[]) {
    try {
        system("bash ../scripts/generate_blacklist.sh");
        
        std::thread udp_thread(run_udp_server);

        std::thread http_thread(run_http_server);

        udp_thread.join();
        http_thread.join();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}