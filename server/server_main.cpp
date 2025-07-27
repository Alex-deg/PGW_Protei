#include "server.h"
#include "../libs/ConfigParser/config_parser.h"

#include <thread>
#include <shared_mutex>
#include <iostream>
#include <chrono>

// Проверка http api
// curl -X POST http://localhost:8080/stop
// curl "http://localhost:8080/check_subscriber?jmsi=123123123123123"


std::mutex udp_data_mutex; 
std::unordered_map<std::string, std::shared_ptr<session>>* sessions = nullptr;
std::shared_ptr<FileHandler> cdr = nullptr;

std::mutex sessions_mutex;

//std::atomic<bool> keep_running(true);  // Флаг для контроля работы потоков
std::atomic<bool> is_shutting_down(false);

void graceful_shutdown(std::unordered_map<std::string, std::shared_ptr<session>>* sessions, 
                       int speed) {
    while (!sessions->empty()) {
        

        std::time_t currentTime = std::time(nullptr);
        std::tm localTime = *std::localtime(&currentTime);
        std::string now = std::ctime(&currentTime);
        now.pop_back();

        std::cout << "shutdowning..." << std::endl;

        for (int i = 0; i < speed && !sessions->empty(); ++i) {
            auto it = sessions->begin();
            std::string imsi = it->first;
            
            cdr->writeLine(now + "," + imsi + "," + "deleted: IMSI = " + imsi);
            
            sessions->erase(it);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    exit(0); 
}

void check_sessions_ttl(int ttl_seconds) {
    while (!is_shutting_down) {
        std::this_thread::sleep_for(std::chrono::seconds(2)); // мб сделать больше задержку
        {
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

    http_server.Post("/stop", [&](const httplib::Request&, httplib::Response& res) {
        std::cout << "DEBUGGING" << std::endl;
        if (is_shutting_down.exchange(true)) {
            res.status = 400;
            res.set_content("Shutdown already in progress\n", "text/plain");
            return;
        }
        graceful_shutdown(sessions, shd_rate);
        res.set_content("Graceful shutdown started\n", "text/plain");
    });

    std::cout << "HTTP Server started on port 8080" << std::endl;
    http_server.listen("0.0.0.0", 8080);
}

// void run_udp_server(UDPServer &server){
//     server.run();
// }

int main(int argc, char* argv[]) {
    try {
        system("bash ../scripts/clear_data_files.sh");
        system("bash ../scripts/generate_blacklist.sh");
    
        std::shared_ptr<config_parser> cp = std::make_shared<config_parser>("../server_config.json");
     
        UDPServer server(cp);

        sessions = server.get_cp_sessions();
        cdr = server.get_cdr_journal();

        std::thread ttl_thread(check_sessions_ttl, cp->get<int>("session_timeout_sec"));
        std::thread http_thread(run_http_server, cp->get<int>("graceful_shutdown_rate"));

        server.run();

        //is_shutting_down = true;

        http_thread.join();
        ttl_thread.join();
      

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
