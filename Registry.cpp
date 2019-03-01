#include "Registry.h"
#include "Log.h"
#include <regex>

template <class T>
T Registry<T>::put(T entry, std::string location) {
    if (entries.count(location) > 0) {
        Log::warnf("Resource at location %s already exists!");
    }
    entries[location] = entry;
    return entry;
}

template <class T>
T Registry<T>::get(std::string location) {
    T *entry = nullptr;

    if (entries.count(location) != 0) {
        entry = entries[location];
    }

    return entry;
}

template<class T>
std::vector<T> Registry<T>::search(std::string pattern) {
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
