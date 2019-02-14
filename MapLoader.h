#ifndef FCWRPG_MAPLOADER_H
#define FCWRPG_MAPLOADER_H


#include <string>
#include "Map.h"

class MapLoader {
public:
    static bool loaded;
    static std::map<std::string, Map *> mapList;

    static Map *getMap(std::string id);
    static void loadMaps();
    static bool loadUnpackedMap(std::string basePath);
    static bool loadPackedMap(std::string file);
};


#endif //FCWRPG_MAPLOADER_H
