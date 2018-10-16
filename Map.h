

#ifndef FCWRPG_MAP_H
#define FCWRPG_MAP_H


#include <allegro5/allegro_font.h>
#include "Tile.h"

struct LinkedText {
    const char *text;
    float x, y;
    ALLEGRO_FONT *font;
    ALLEGRO_COLOR color;
    LinkedText *next;
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
    LinkedText *texts;
    void (*handlerFunction)(ALLEGRO_EVENT event);

    Map(int id, const char *name, Tile **tileset, int **tilemap, int length, int height);

    void draw();
    void addSprite(Sprite *sprite);
    void addText(const char *text, ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y);
    Sprite* getSpriteById(const char *id);
    void handleEvent(ALLEGRO_EVENT event);
    void setEventHandlerFunction(void (*handler)(ALLEGRO_EVENT event));
};

#endif //FCWRPG_MAP_H
