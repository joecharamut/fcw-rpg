#ifndef FCWRPG_LOG_H
#define FCWRPG_LOG_H

#include <string>
#include <vector>
#include "Options.h"

class Log {
public:
    static void debug(const std::string &message) { if (Options::Runtime::debug) log(message, DEBUG); };
    static void info (const std::string &message) { log(message, INFO); };
    static void warn (const std::string &message) { log(message, WARN); };
    static void error(const std::string &message) { log(message, ERROR); };

private:
    enum LogLevel {
        INFO,
        WARN,
        ERROR,
        DEBUG
    };
    static std::vector<std::string> levelStrings;

    static void log(std::string message, LogLevel level);
};

#endif //FCWRPG_LOG_H
