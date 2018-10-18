#include "Sprite.h"
#include "Globals.h"
#include <cstdarg>

Sprite::Sprite(float x, float y, const char *id, ALLEGRO_BITMAP *image) {
    this->x = x;
    this->y = y;
    this->frames[nextFrameStore++] = image;
    this->width = al_get_bitmap_width(image);
    this->height = al_get_bitmap_height(image);
    this->id = id;
    this->speed = 1;
    this->numFrames++;
}

Sprite::Sprite(float x, float y, const char *id, int frameCount, ...) {

}

void Sprite::draw() {
     ++speedCount;
     if (speedCount >= speed) {
         speedCount = 0;
         ++currentFrame %= numFrames;
         speedCount = 0;
     }
     al_draw_bitmap(frames[currentFrame], x, y, 0);
}

void Sprite::setX(float newX) {
    this->x = newX;
}
void Sprite::setY(float newY) {
    this->y = newY;
}

void Sprite::addFrame(ALLEGRO_BITMAP *image) {
    frames[nextFrameStore++] = image;
    numFrames++;
}

