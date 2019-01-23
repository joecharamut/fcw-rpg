

#ifndef FCWRPG_UTIL_H
#define FCWRPG_UTIL_H


#include <string>
#include <vector>

class Util {
public:
    static bool checkInt(std::string test);
    static std::vector<std::string> splitString(std::string str, std::string delimiter);

    static long long int getMilliTime();
};


#endif //FCWRPG_UTIL_H
