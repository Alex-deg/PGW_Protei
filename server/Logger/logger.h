#pragma once

#include <iostream>
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
    // Уровни логирования
    enum class Level {
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    // Инициализация логгера (консоль + файл)
    static void init(const std::string& logFilePath = "logs/log.txt");

    // Логирование с указанным уровнем
    static void log(Level level, const std::string& message);

    // Удобные методы для каждого уровня
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warn(const std::string& message);
    static void error(const std::string& message);
    static void critical(const std::string& message);
    
private:
    static std::shared_ptr<spdlog::logger> logger_;
};