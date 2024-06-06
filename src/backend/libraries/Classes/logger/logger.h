#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    static Logger& getInstance();

    void setLogFile(const std::string& filename);
    void logInfo(const std::string& message);
    void logWarning(const std::string& message);
    void logError(const std::string& message);

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string getCurrentTime();
    void log(LogLevel level, const std::string& message);

    std::ofstream logFile;
    std::mutex logMutex;
};

#endif // LOGGER_H
