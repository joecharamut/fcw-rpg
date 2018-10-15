#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#ifndef FCWRPG_SPRITE_H
#define FCWRPG_SPRITE_H

class Sprite {
public:
    float x;
    float y;
    ALLEGRO_BITMAP *image;
    int width;
    int height;
    const char *id;

    Sprite(float x, float y, ALLEGRO_BITMAP *image, const char *id);
    Sprite(float x, float y, ALLEGRO_BITMAP *image);
    virtual void draw();
    void setX(float newX);
    void setY(float newY);
};

struct LinkedSprite {
    Sprite *sprite;
    struct LinkedSprite *next;
};

#endif //FCWRPG_SPRITE_H
