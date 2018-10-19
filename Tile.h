

#ifndef FCWRPG_TILE_H
#define FCWRPG_TILE_H

#include "Sprite.h"

enum COLLISION_TYPE {
    NONE,
    TILE,
    BOX
};

class Tile : public Sprite {
public:
    COLLISION_TYPE collision;

    Tile(ALLEGRO_BITMAP *image, COLLISION_TYPE collision);
    explicit Tile(ALLEGRO_BITMAP *image) : Tile(image, NONE) {};
    Tile(const Tile &tile) : Tile(tile.frames[0]) {};
    void draw() override;
};


#endif //FCWRPG_TILE_H
