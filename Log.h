#ifndef FCWRPG_LOG_H
#define FCWRPG_LOG_H

#include <string>
#include <vector>
#include "Options.h"

class Log {
public:
    static void debug(const std::string &message) { if (Options::Runtime::debug) Log::log(LOG_DEBUG, message); };
    static void info (const std::string &message) { Log::log(LOG_INFO, message); };
    static void warn (const std::string &message) { Log::log(LOG_WARN, message); };
    static void error(const std::string &message) { Log::log(LOG_ERROR, message); };

    static void debugf(const char *fmt, ...);
    static void infof (const char *fmt, ...);
    static void warnf (const char *fmt, ...);
    static void errorf(const char *fmt, ...);

private:
    enum LogLevel {
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_DEBUG
    };
    static std::vector<std::string> levelStrings;

    static void log(LogLevel level, std::string message);
    static void logf(LogLevel level, const char *fmt, va_list args);
};

#endif //FCWRPG_LOG_H
