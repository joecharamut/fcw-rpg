

#ifndef FCWRPG_UTIL_H
#define FCWRPG_UTIL_H


#include <string>
#include <vector>

class Util {
public:
    static bool checkInt(std::string test);
    static std::vector<std::string> splitString(std::string str, std::string delimiter);

    template <typename T>
    static bool vectorContains(std::vector<T> vector, T object) {
        for (const auto &item : vector) {
            if (item == object) {
                return true;
            }
        }
        return false;
    }

    static long long int getMilliTime();
};


#endif //FCWRPG_UTIL_H
