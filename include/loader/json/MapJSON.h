#ifndef FCWRPG_MAPJSON_H
#define FCWRPG_MAPJSON_H

#include <string>
#include <vector>
#include <Map.h>

#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

class MapJSON {
public:
    std::string id;
    std::vector<std::vector<std::vector<int>>> tilemap;
    std::vector<std::string> tileset;
    std::vector<std::string> objects;

    MapJSON() = default;

    Map *construct() {
        return new Map(id, tileset, tilemap, objects);
    }

    template <class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(id),
                CEREAL_NVP(tilemap),
                CEREAL_NVP(tileset),
                CEREAL_NVP(objects)
        );
    }
};


#endif //FCWRPG_MAPJSON_H
