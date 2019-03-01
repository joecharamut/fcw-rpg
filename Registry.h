#ifndef FCWRPG_REGISTRY_H
#define FCWRPG_REGISTRY_H

#include <string>
#include <vector>
#include <map>

template <class T>
class Registry {
public:
    T put(T entry, std::string location);
    T get(std::string location);
    std::vector<T> search(std::string pattern);

private:
    std::map<std::string, T> entries = {};
};


#endif //FCWRPG_REGISTRY_H
