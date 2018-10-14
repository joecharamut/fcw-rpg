

#ifndef FCWRPG_TILE_H
#define FCWRPG_TILE_H


#include "Sprite.h"

class Tile : public Sprite {
public:

    Tile(ALLEGRO_BITMAP *image);
    void draw(float x, float y);
};


#endif //FCWRPG_TILE_H
