#include <utility>
#include <iostream>
#include <chrono>

#include "Util.h"

void Util::log(std::string message, std::string src, LogLevel level) {
    std::cout << "[" << src << (src.empty() ? "" : "/") << levelStrings[level] << "]: " << message << std::endl;
}

void Util::log(std::string message, std::string src) {
    log(std::move(message), std::move(src), INFO);
}

void Util::log(std::string message, LogLevel level) {
    log(std::move(message), "", level);
}

void Util::log(std::string message) {
    log(std::move(message), "", INFO);
}

bool Util::checkInt(std::string test) {
    for (char c : test) if (!isdigit(c) && c != '-') return false;
    return true;
}

std::vector<std::string> Util::splitString(std::string str, std::string delimiter) {
    std::string modStr = str;
    size_t pos = 0;
    std::vector<std::string> split = {};
    std::string token;
    while ((pos = modStr.find(delimiter)) != std::string::npos) {
        token = modStr.substr(0, pos);
        split.push_back(token);
        modStr.erase(0, pos + delimiter.length());
    }
    split.push_back(modStr);
    return split;
}

using namespace std;

long long int Util::getMilliTime() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
}