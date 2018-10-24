

#ifndef FCWRPG_TILE_H
#define FCWRPG_TILE_H

#include "Sprite.h"
#include "cereal/cereal.hpp"

enum COLLISION_TYPE {
    NONE,
    TILE,
    BOX
};

class Tile : public Sprite {

public:
    COLLISION_TYPE collision;

    Tile(const char *image, COLLISION_TYPE collision);
    explicit Tile(const char *image) : Tile(image, NONE) {};
    Tile(const Tile &tile) : Tile(tile.imageName) {};
    void draw() override;

    template <class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(collision),
                CEREAL_NVP(imageName)
        );
    }
};


#endif //FCWRPG_TILE_H
