#include "Log.h"
#include <iostream>
#include <memory>
#include <cstdarg>

std::vector<std::string> Log::levelStrings = {
        "INFO", "WARN", "ERROR", "DEBUG"
};

void Log::log(LogLevel level, std::string message) {
    bool sout = (level == LOG_DEBUG || level == LOG_INFO);
    (sout ? std::cout : std::cerr) << "[" << levelStrings[level] << "]: " << message << std::endl;
}

void Log::debugf(const char *fmt, ...) {
    if (Options::Runtime::debug) {
        va_list args;
        va_start(args, fmt);
        logf(LOG_DEBUG, fmt, args);
        va_end(args);
    }
}

void Log::infof(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logf(LOG_INFO, fmt, args);
    va_end(args);
}

void Log::warnf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logf(LOG_WARN, fmt, args);
    va_end(args);
}

void Log::errorf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logf(LOG_ERROR, fmt, args);
    va_end(args);
}

void Log::logf(LogLevel level, const char *fmt, va_list args) {
    size_t size = (size_t) vsnprintf(nullptr, 0, fmt, args) + 1;
    char *buf = new char[size];
    vsnprintf(buf, size, fmt, args);

    log(level, std::string(buf, buf + size - 1));
}
