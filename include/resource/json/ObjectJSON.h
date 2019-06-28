#ifndef FCWRPG_OBJECTJSON_H
#define FCWRPG_OBJECTJSON_H

#include <string>
#include <map>
#include <object/Object.h>

#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/map.hpp>

class ObjectJSON {
public:
    std::string id;
    float x;
    float y;
    std::map<std::string, std::string> sprites;
    CollisionType collision;

    Object *construct() {
        return new Object(id, x, y, sprites, collision);
    }

    template <class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(id),
                CEREAL_NVP(x),
                CEREAL_NVP(y),
                CEREAL_NVP(sprites),
                CEREAL_NVP(collision)
        );
    }
};


#endif //FCWRPG_OBJECTJSON_H
