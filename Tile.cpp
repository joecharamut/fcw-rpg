

#include "Tile.h"

Tile::Tile(std::string image, COLLISION_TYPE collision) : Sprite(0,0,image) {
    this->collision = collision;
}

void Tile::draw()  {
    Sprite::draw();
}