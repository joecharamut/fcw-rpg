#ifndef FCWRPG_MAPLOADER_H
#define FCWRPG_MAPLOADER_H


#include <string>
#include "Map.h"

class MapLoader {
public:
    static bool loaded;
    static std::map<std::string, Map *> mapList;

    static Map *getMap(std::string id);
    static bool loadMaps();
private:
    static bool processMap(std::string id);
    static std::string processUnpackedResources(std::string basePath);
    static std::string processPackedResources(std::string file);
};


#endif //FCWRPG_MAPLOADER_H
