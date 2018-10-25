#include <utility>

#include "Sprite.h"
#include "Globals.h"
#include <cstdarg>
#include <cstdio>

Sprite::Sprite(float x, float y, std::string id, std::string image) {
    this->x = x;
    this->y = y;
    //this->frames.push_back(al_load_bitmap(image.c_str()));
    addFrame(image);
    this->imageName = image;
    this->width = al_get_bitmap_width(frames.back());
    this->height = al_get_bitmap_height(frames.back());
    this->id = std::move(id);
    this->speed = 1;
    this->boundingBox = new BoundingBox(x, y, x+width, y+height);
}

//Sprite::Sprite(float x, float y, const char *id, int frameCount, ...) {
//
//}

void Sprite::draw() {
     ++speedCount;
     if (speedCount >= speed) {
         speedCount = 0;
         ++currentFrame %= frames.size();
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

void Sprite::addFrame(std::string image) {
    frames.push_back(al_load_bitmap(image.c_str()));
    frameStr.push_back(image);
}

void Sprite::updateBoundingBox() {
    delete boundingBox;
    this->boundingBox = new BoundingBox(x, y, x+width, y+height);
}

