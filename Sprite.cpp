#include <utility>

#include <utility>

#include <utility>

#include <utility>

#include "Sprite.h"
#include "Globals.h"
#include <cstdarg>
#include <cstdio>

Sprite::Sprite(float x, float y, std::string id, std::vector<Animation> frames) {
    this->x = x;
    this->y = y;
    this->frames = std::move(frames);
    this->frames[0].loadFrames();
    this->width = al_get_bitmap_width(this->frames[0].loadedFrames[0]);
    this->height = al_get_bitmap_height(this->frames[0].loadedFrames[0]);
    this->id = std::move(id);
    this->boundingBox = new BoundingBox(x, y, x+width, y+height);
    this->collision = NONE;
}

void Sprite::draw() {
     al_draw_bitmap(frames[animation].nextFrame(), x, y, 0);
}

void Sprite::setX(float newX) {
    this->x = newX;
    updateBoundingBox();
}
void Sprite::setY(float newY) {
    this->y = newY;
    updateBoundingBox();
}

void Sprite::updateBoundingBox() {
    delete boundingBox;
    this->boundingBox = new BoundingBox(x, y, x+width, y+height);
}

