#include "logger.h"

std::shared_ptr<spdlog::logger> Logger::logger_ = nullptr;

void Logger::init(const std::string& logFilePath) {
    try {
        // Создаём сенки (консоль + файл)
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);

        // Комбинируем сенки
        spdlog::sinks_init_list sinks = {consoleSink, fileSink};

        // Создаём логгер
        logger_ = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(), sinks.end());
        spdlog::register_logger(logger_);

        // Устанавливаем уровень логирования (можно менять)
        logger_->set_level(spdlog::level::debug);
        logger_->flush_on(spdlog::level::err);  // Флашим логи при ошибках

        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
    }
}

void Logger::log(Level level, const std::string& message) {
    if (!logger_) {
        init();  // Автоинициализация, если забыли вызвать init()
    }

    switch (level) {
        case Level::Debug:    logger_->debug(message);    break;
        case Level::Info:     logger_->info(message);     break;
        case Level::Warn:     logger_->warn(message);     break;
        case Level::Error:    logger_->error(message);    break;
        case Level::Critical: logger_->critical(message); break;
    }
}

// Удобные обёртки
void Logger::debug(const std::string& message)    { log(Level::Debug, message); }
void Logger::info(const std::string& message)      { log(Level::Info, message); }
void Logger::warn(const std::string& message)      { log(Level::Warn, message); }
void Logger::error(const std::string& message)     { log(Level::Error, message); }
void Logger::critical(const std::string& message)  { log(Level::Critical, message); }