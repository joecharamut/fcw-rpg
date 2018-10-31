#include <utility>
#include <iostream>

#include "Util.h"

void Util::log(std::string message, std::string src, LogLevel level) {
    std::cout << "[" << src << (src.empty() ? "" : "/") << levelStrings[level] << "]: " << message << std::endl;
}

void Util::log(std::string message, std::string src) {
    log(std::move(message), std::move(src), INFO);
}

void Util::log(std::string message) {
    log(std::move(message), "", INFO);
}

void Util::error(std::string message) {

}
