#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

class Logger {
public:
    Logger(const std::string& filename);
    ~Logger();

    void log(const std::string& message);
    void logError(const std::string& message);
    void logWarning(const std::string& message);

private:
    std::ofstream logFile;

    std::string getCurrentTime();
};

#endif // LOGGER_H