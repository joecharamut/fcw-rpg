#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "BoundingBox.h"

#ifndef FCWRPG_SPRITE_H
#define FCWRPG_SPRITE_H

class Sprite {
public:
    float x;
    float y;
    ALLEGRO_BITMAP **frames = (ALLEGRO_BITMAP **) calloc(64, sizeof(ALLEGRO_BITMAP *));
    int nextFrameStore = 0;
    int width;
    int height;
    const char *id;

    int numFrames = 0;
    int currentFrame = 0;
    int speed = 0;
    int speedCount = 0;

    Sprite(float x, float y, const char *id, ALLEGRO_BITMAP *image);
    Sprite(float x, float y, ALLEGRO_BITMAP *image) : Sprite(x, y, nullptr, image) {};
    Sprite(float x , float y, const char *id, int frameCount, ...);
    Sprite(float x , float y, int frameCount, ...) : Sprite(x, y, nullptr, frameCount) {};
    virtual void draw();
    virtual void setX(float newX);
    virtual void setY(float newY);
    BoundingBox *boundingBox;
    void updateBoundingBox();
    void addFrame(ALLEGRO_BITMAP *image);
};

struct LinkedSprite {
    Sprite *sprite;
    struct LinkedSprite *next;
};

#endif //FCWRPG_SPRITE_H
