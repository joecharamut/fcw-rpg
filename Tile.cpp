

#include "Tile.h"

Tile::Tile(ALLEGRO_BITMAP *image, COLLISION_TYPE collision) : Sprite(0,0,image) {
    this->collision = collision;
}

void Tile::draw(float x, float y)  {
    setX(x);
    setY(y);
    Sprite::draw();
}