#include <utility>
#include <iostream>
#include <chrono>

#include "Util.h"

bool Util::checkInt(std::string test) {
    for (char c : test) if (!isdigit(c) && c != '-') return false;
    return true;
}

std::vector<std::string> Util::splitString(std::string str, std::string delimiter) {
    std::vector<std::string> out = {};
    size_t start, end = 0;
    while ((start = str.find_first_not_of(delimiter, end)) != std::string::npos) {
        end = str.find(delimiter, start);
        out.push_back(str.substr(start, end-start));
    }
    return out;
}

using namespace std;

long long int Util::getMilliTime() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
}
