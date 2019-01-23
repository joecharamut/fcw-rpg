#include <iostream>
#include "Log.h"

std::vector<std::string> Log::levelStrings = {
        "INFO", "WARN", "ERROR", "DEBUG"
};

void Log::log(std::string message, LogLevel level) {
    std::cout << "[" << levelStrings[level] << "]: " << message << std::endl;
}
