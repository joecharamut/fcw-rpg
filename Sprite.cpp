#include "Sprite.h"
#include "Globals.h"
#include <cstdarg>
#include <cstdio>

Sprite::Sprite(float x, float y, const char *id, const char *image) {
    this->x = x;
    this->y = y;
    this->frames[nextFrameStore++] = al_load_bitmap(image);
    this->imageName = (char *) image;
    this->width = al_get_bitmap_width(frames[nextFrameStore-1]);
    this->height = al_get_bitmap_height(frames[nextFrameStore-1]);
    this->id = id;
    this->speed = 1;
    this->numFrames++;
    this->boundingBox = new BoundingBox(x, y, x+width, y+height);
}

//Sprite::Sprite(float x, float y, const char *id, int frameCount, ...) {
//
//}

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
    updateBoundingBox();
}
void Sprite::setY(float newY) {
    this->y = newY;
    updateBoundingBox();
}

void Sprite::addFrame(ALLEGRO_BITMAP *image) {
    frames[nextFrameStore++] = image;
    numFrames++;
}

void Sprite::updateBoundingBox() {
    delete boundingBox;
    this->boundingBox = new BoundingBox(x, y, x+width, y+height);
}

