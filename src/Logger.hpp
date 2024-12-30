#pragma once
#include <iostream>
#include <string>

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

inline void Log(const std::string& message, LogLevel level = LogLevel::INFO) {
    switch (level) {
        case LogLevel::DEBUG: std::cout << "[DEBUG] "; break;
        case LogLevel::INFO:  std::cout << "[INFO] ";  break;
        case LogLevel::WARN:  std::cout << "[WARN] ";  break;
        case LogLevel::ERROR: std::cout << "[ERROR] "; break;
    }
    std::cout << message << std::endl;
}
