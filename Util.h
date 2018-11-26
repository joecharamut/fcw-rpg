

#ifndef FCWRPG_UTIL_H
#define FCWRPG_UTIL_H


#include <string>
#include <vector>

enum LogLevel {
    INFO,
    WARN,
    ERROR,
    DEBUG
};

const static std::vector<std::string> levelStrings = {
        "INFO", "WARN", "ERROR", "DEBUG"
};

class Util {
public:
    static void log(std::string message, std::string src, LogLevel level);
    static void log(std::string message, std::string src);
    static void log(std::string message, LogLevel level);
    static void log(std::string message);

    static bool checkInt(std::string test);
    static std::vector<std::string> splitString(std::string str, std::string delimiter);
};


#endif //FCWRPG_UTIL_H
