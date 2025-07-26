#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <memory>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

class Logger {
public:

    enum class Level {
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    static void init(const std::string& logFilePath = "logs/log.txt");

    static void log(Level level, const std::string& message);
    
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warn(const std::string& message);
    static void error(const std::string& message);
    static void critical(const std::string& message);
    static void Logger::set_level(const std::string& level);

private:
    static std::shared_ptr<spdlog::logger> logger_;
};