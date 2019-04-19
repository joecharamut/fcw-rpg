#include <iostream>
#include <memory>
#include <cstdarg>

#include <Log.h>

std::vector<std::string> Log::levelStrings = {
        "INFO", "WARN", "ERROR", "DEBUG", "VERBOSE"
};

void Log::log(LogLevel level, std::string message) {
    bool err = (level == LOG_WARN || level == LOG_ERROR);
    (err ? std::cerr : std::cout) << "[" << levelStrings[level] << "]: " << message << std::endl;
}

void Log::verbosef(const char *fmt, ...) {
    if (Options::Runtime::verbose) {
        va_list args;
        va_start(args, fmt);
        logf(LOG_VERBOSE, fmt, args);
        va_end(args);
    }
}

void Log::debugf(const char *fmt, ...) {
    if (Options::Runtime::debug || Options::Runtime::verbose) {
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
    va_list copy;
    va_copy(copy, args);
    size_t size = (size_t) vsnprintf(nullptr, 0, fmt, copy) + 1;
    auto *buf = new char[size];
    vsnprintf(buf, size, fmt, args);
    va_end(copy);
    log(level, std::string(buf, buf + size - 1));
}
