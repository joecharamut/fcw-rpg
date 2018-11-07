#include <utility>


#include "Tile.h"

Tile::Tile(Animation image, COLLISION_TYPE collision) : Sprite(0,0, std::move(image)) {
    this->collision = collision;
}