#include "logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>
#include "../../mysql-queries/mysql-queries.h"

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> guard(logMutex);
    if (logFile.is_open()) {
        logFile.close();
    }
    logFile.open(filename, std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Не удалось открыть файл для логирования: " << filename << std::endl;
    }
}

void Logger::logInfo(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::logWarning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::logError(const std::string& message) {
    log(LogLevel::ERROR, message);
}

std::string Logger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time);

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> guard(logMutex);
    std::string prefix;
    switch (level) {
        case LogLevel::INFO:
            prefix = "[INFO] ";
            break;
        case LogLevel::WARNING:
            prefix = "[WARNING] ";
            break;
        case LogLevel::ERROR:
            prefix = "[ERROR] ";
            break;
    }

    std::string logMessage = getCurrentTime() + " " + prefix + message;
//    std::cout << std::left << getCurrentTime() << " " << std::setw(10) <<  prefix << message << '\n';

    std::vector<std::map<std::string, std::string>> data = {
            {{"timestamp", getCurrentTime()}, {"type", prefix},
             {"message", message}}};
    ExecuteInsertQuery("library", "insert", "logging", data);
    if (logFile.is_open()) {
        logFile << std::left << getCurrentTime() << " " << std::setw(10) <<  prefix << message << '\n';
    }
}
