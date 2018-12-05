#include <utility>

#include "Sprite.h"
#include <cstdarg>
#include <cstdio>

// Constructor
Sprite::Sprite(float x, float y, std::string id, std::vector<Animation> frames) {
    // Set values
    this->x = x;
    this->y = y;
    this->frames = std::move(frames);
    this->frames[0].loadFrames();
    this->width = al_get_bitmap_width(this->frames[0].loadedFrames[0]);
    this->height = al_get_bitmap_height(this->frames[0].loadedFrames[0]);
    this->id = std::move(id);
    this->boundingBox = new BoundingBox(x, y, x+width, y+height);
}

// Function to draw the sprite
void Sprite::draw() {
    // Call draw function with the next animation frame and the coordinates of the sprite
     al_draw_bitmap(frames[animation].nextFrame(), x-dX, y-dY, 0);
}

void Sprite::setX(float newX) {
    this->x = newX;
    updateBoundingBox();
}
void Sprite::setY(float newY) {
    this->y = newY;
    updateBoundingBox();
}

void Sprite::setDisplace(float dX, float dY) {
    this->dX = dX;
    this->dY = dY;
    updateBoundingBox();
}

void Sprite::updateBoundingBox() {
    delete boundingBox;
    this->boundingBox = new BoundingBox(x-dX, y-dY, x-dX+width, y-dY+height);
}

