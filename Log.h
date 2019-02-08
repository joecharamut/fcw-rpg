#ifndef FCWRPG_LOG_H
#define FCWRPG_LOG_H

#include <string>
#include <vector>
#include "Options.h"

class Log {
public:
    static void debug(const std::string &message) { if (Options::Runtime::debug) Log::log(message, LOG_DEBUG); };
    static void info (const std::string &message) { Log::log(message, LOG_INFO); };
    static void warn (const std::string &message) { Log::log(message, LOG_WARN); };
    static void error(const std::string &message) { Log::log(message, LOG_ERROR); };

private:
    enum LogLevel {
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_DEBUG
    };
    static std::vector<std::string> levelStrings;

    static void log(std::string message, LogLevel level);
};

#endif //FCWRPG_LOG_H
