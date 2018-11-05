#include <utility>

#include <utility>

#include <utility>

#include "Sprite.h"
#include "Globals.h"
#include <cstdarg>
#include <cstdio>

Sprite::Sprite(float x, float y, std::string id, std::string image) {
    this->x = x;
    this->y = y;
    addFrame(std::move(image));
    this->width = al_get_bitmap_width(frames[0]);
    this->height = al_get_bitmap_height(frames[0]);
    this->id = std::move(id);
    this->speed = 1;
    this->boundingBox = new BoundingBox(x, y, x+width, y+height);
    this->collision = NONE;
}

Sprite::Sprite(float x, float y, std::string id, std::vector<std::string> frames, int speed) : Sprite(x, y, std::move(id), frames[0]) {
    for (int i  = 1; i < frames.size(); i++) {
        addFrame(frames[i]);
    }
    this->speed = speed;
}

void Sprite::draw() {
     ++speedCount;
     if (speedCount >= speed) {
         speedCount = 0;
         ++currentFrame %= frames.size();
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

