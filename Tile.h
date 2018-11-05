#include <utility>


#ifndef FCWRPG_TILE_H
#define FCWRPG_TILE_H

#include "Sprite.h"
#include "cereal/cereal.hpp"

class Tile : public Sprite {

public:
    Tile(std::string image, COLLISION_TYPE collision);
    explicit Tile(std::string image) : Tile(std::move(image), NONE) {};
    Tile() = default;;
    Tile(const Tile &tile) : Tile(tile.frameStr[0]) {};
    void draw() override;
};


#endif //FCWRPG_TILE_H
