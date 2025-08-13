#include "logger.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

class Logger {
public:
    Logger(const std::string& filename) : logFile(filename, std::ios::app) {
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(const std::string& message) {
        if (logFile.is_open()) {
            logFile << getCurrentTime() << " - " << message << std::endl;
        }
    }

private:
    std::ofstream logFile;

    std::string getCurrentTime() {
        std::time_t now = std::time(nullptr);
        char buf[100];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return buf;
    }
};