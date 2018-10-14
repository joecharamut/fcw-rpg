

#include "Tile.h"

Tile::Tile(ALLEGRO_BITMAP *image) : Sprite(0,0,image) {

}

void Tile::draw(float x, float y)  {
    setX(x);
    setY(y);
    Sprite::draw();
}