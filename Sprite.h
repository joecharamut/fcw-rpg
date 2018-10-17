#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#ifndef FCWRPG_SPRITE_H
#define FCWRPG_SPRITE_H

class LinkedImage;
class LinkedSprite;

class Sprite {
public:
    float x;
    float y;
    LinkedImage *frames;
    int width;
    int height;
    const char *id;
    int numFrames;
    int currentFrame;
    int speed;

    Sprite(float x, float y, const char *id, ALLEGRO_BITMAP *image);
    Sprite(float x, float y, ALLEGRO_BITMAP *image) : Sprite(x, y, nullptr, image) {};
    Sprite(float x , float y, const char *id, int frameCount, ...);
    Sprite(float x , float y, int frameCount, ...) : Sprite(x, y, nullptr, frameCount) {};
    virtual void draw();
    virtual void setX(float newX);
    virtual void setY(float newY);
    void addFrame(ALLEGRO_BITMAP *image);
};

struct LinkedSprite {
    Sprite *sprite;
    struct LinkedSprite *next;
};

struct LinkedImage {
    ALLEGRO_BITMAP *image;
    int frameNum = 0;
    struct LinkedImage *next;
};

#endif //FCWRPG_SPRITE_H
