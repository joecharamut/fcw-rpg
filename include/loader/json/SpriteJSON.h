#ifndef FCWRPG_SPRITEJSON_H
#define FCWRPG_SPRITEJSON_H

#include <string>
#include <vector>
#include <Sprite.h>

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

class SpriteJSON {
public:
    std::string id;
    std::vector<std::string> frames;
    int speed = 1;

    SpriteJSON() = default;

    Sprite *construct() {
        return new Sprite(id, frames, speed);
    }

    template<class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(id),
                CEREAL_NVP(frames),
                CEREAL_NVP(speed)
        );
    }

};

#endif //FCWRPG_SPRITEJSON_H
