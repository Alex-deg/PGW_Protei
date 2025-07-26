#include "server.h"
#include "ConfigParser/config_parser.h"

#include <thread>

// Возможно лучше здесь распарсить конфиг и просто передавать настройки в конструктор 

// добавить mutex
std::mutex udp_data_mutex; 
std::unordered_map<std::string, std::shared_ptr<session>>* sessions;

std::mutex sessions_mutex;

void check_sessions_ttl(int ttl_seconds) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2)); // мб сделать больше задержку

        std::lock_guard<std::mutex> lock(sessions_mutex);
        auto now = std::chrono::steady_clock::now();
        
        std::cout << "==========================" << std::endl;

        for (auto it = sessions->begin(); it != sessions->end(); ) {
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(
                now - it->second->getCreatedTime()
            ).count();
            std::cout << it->first << std::endl;
            if (duration >= ttl_seconds) {
                std::cout << "Session expired: " << it->first << std::endl;
                it = sessions->erase(it);
            } else {
                ++it;
            }
        }

        std::cout << "==========================" << std::endl;

    }
}

void run_http_server(int shd_rate) {

    httplib::Server http_server;

    http_server.Get("/check_subscriber", [](const httplib::Request& req, httplib::Response& res) {
        std::lock_guard<std::mutex> lock(udp_data_mutex); 
        auto jmsi = req.get_param_value("jmsi");
        if (sessions->find(jmsi) != sessions->end()) {
            res.set_content("active\n", "text/plain"); 
        } else {
            res.status = 404;
            res.set_content("Session not found\n", "text/plain");
        }
    });

    http_server.Post("/stop", [](const httplib::Request& req, httplib::Response& res) {
        static std::atomic<bool> is_shutting_down(false);
        if (is_shutting_down) {
            res.status = 400;
            res.set_content("Shutdown already in progress\n", "text/plain");
            return;
        }
        is_shutting_down = true;
        std::thread([]() { exit(0); }).detach(); // Упрощенный shutdown реализовать посложнее
        res.set_content("Graceful shutdown started\n", "text/plain");
    });

    std::cout << "HTTP Server started on port 8080" << std::endl;
    http_server.listen("0.0.0.0", 8080);
}

void run_udp_server(config_parser &cp){
    UDPServer server(sessions, cp);
    server.run();
}

int main(int argc, char* argv[]) {
    try {
        system("bash ../scripts/clear_data_files.sh");
        system("bash ../scripts/generate_blacklist.sh");
        
        config_parser cp("server_config.json");
        
        std::thread ttl_thread(check_sessions_ttl, cp.get<int>("session_timeout_sec"));
        ttl_thread.detach();

        std::thread udp_thread(run_udp_server, cp);

        std::thread http_thread(run_http_server, cp.get<int>("graceful_shutdown_rate"));

        udp_thread.join();
        http_thread.join();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}