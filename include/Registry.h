#ifndef FCWRPG_REGISTRY_H
#define FCWRPG_REGISTRY_H

#include <string>
#include <vector>
#include <map>
#include <regex>
#include "Log.h"

template <class T>
class Registry {
public:
    static_assert(std::is_pointer<T>::value, "Registry Type Must be a Pointer");

    ~Registry() {
        for (auto entry : entries) {
            delete entry.second;
        }
    }

    T put(T entry, std::string location) {
        if (entries.count(location) > 0) {
            Log::warnf("Resource at location %s already exists!", location.c_str());
        }
        entries[location] = entry;
        return entry;
    }

    T get(std::string location) {
        T entry = nullptr;

        if (entries.count(location) != 0) {
            entry = entries[location];
        }

        return entry;
    }

    std::vector<T> search(const std::string &pattern) {
        std::vector<T> results;
        std::regex regex(pattern);

        for (const auto &entry : entries) {
            std::string loc = entry.first;
            if (std::regex_match(loc, regex)) {
                results.push_back(entry.second);
            }
        }

        return results;
    }

    typename std::map<std::string, T>::iterator begin() {
        return entries.begin();
    }

    typename std::map<std::string, T>::iterator end() {
        return entries.end();
    }


private:
    std::map<std::string, T> entries = {};
};

#endif //FCWRPG_REGISTRY_H
