#include "logger.h"

std::shared_ptr<spdlog::logger> Logger::logger_ = nullptr;

void Logger::init(const std::string& logFilePath) {
    try {
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);

        spdlog::sinks_init_list sinks = {consoleSink, fileSink};

        fileSink->set_level(spdlog::level::info);

        logger_ = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(), sinks.end());
        spdlog::register_logger(logger_);

        logger_->set_level(spdlog::level::info);
        logger_->flush_on(spdlog::level::err);

        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
    }
}

void Logger::set_level(const std::string& level){
    std::string lv = "";
    for(int i = 0; i < level.size(); i++){
        lv += std::tolower(level[i]);
    }
    std::cout << lv << std::endl;
    if(lv == "debug"){
        logger_->set_level(spdlog::level::debug);
        return;
    }
    if(lv == "info"){
        logger_->set_level(spdlog::level::info);
        return;
    }
    if(lv == "warn"){
        logger_->set_level(spdlog::level::warn);
        return;
    }
    if(lv == "error"){
        logger_->set_level(spdlog::level::err);
        return;
    }
    if(lv == "critical"){
        logger_->set_level(spdlog::level::critical);
        return;
    }
    throw std::invalid_argument("Некорректное значение уровня логирования");
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

    logger_->flush();

}

void Logger::debug(const std::string& message)     { log(Level::Debug, message); }
void Logger::info(const std::string& message)      { log(Level::Info, message); }
void Logger::warn(const std::string& message)      { log(Level::Warn, message); }
void Logger::error(const std::string& message)     { log(Level::Error, message); }
void Logger::critical(const std::string& message)  { log(Level::Critical, message); }