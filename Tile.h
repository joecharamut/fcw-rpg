#include <utility>


#ifndef FCWRPG_TILE_H
#define FCWRPG_TILE_H

#include "Sprite.h"
#include "cereal/cereal.hpp"

class Tile : public Sprite {

public:
    Tile(Animation image, COLLISION_TYPE collision);
    explicit Tile(Animation image) : Tile(std::move(image), NONE) {};
    Tile() = default;;
    Tile(const Tile &tile) : Tile(tile.frames[0]) {};
};


#endif //FCWRPG_TILE_H
