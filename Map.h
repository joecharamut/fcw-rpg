

#ifndef FCWRPG_MAP_H
#define FCWRPG_MAP_H


#include "Tile.h"

struct LinkedSprite {
    Sprite *sprite;
    struct LinkedSprite *next;
};

class Map {
public:
    int id;
    const char *name;
    Tile **tileset;
    int **tilemap;
    int length;
    int height;
    LinkedSprite *sprites;

    Map(int id, const char *name, Tile **tileset, int **tilemap, int length, int height);

    void draw();
    void addSprite(Sprite *sprite);
    Sprite* getSpriteById(const char *id);

};

#endif //FCWRPG_MAP_H
