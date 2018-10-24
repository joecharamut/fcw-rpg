

#include "Tile.h"

Tile::Tile(const char *image, COLLISION_TYPE collision) : Sprite(0,0,image) {
    this->collision = collision;
}

void Tile::draw()  {
    Sprite::draw();
}