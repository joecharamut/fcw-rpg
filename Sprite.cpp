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

// Set X coordinate of the sprite
void Sprite::setX(float newX) {
    this->x = newX;
    // And update the bounding box
    updateBoundingBox();
}

// Set Y coordinate
void Sprite::setY(float newY) {
    this->y = newY;
    // And update the bounding box
    updateBoundingBox();
}

// Set the displacement values for when the screen scrolls
void Sprite::setDisplace(float dX, float dY) {
    this->dX = dX;
    this->dY = dY;
    // And update the bounding box
    updateBoundingBox();
}

// Function to update the bounding box
void Sprite::updateBoundingBox() {
    // Delete the old one
    delete boundingBox;
    // Create new one
    this->boundingBox = new BoundingBox(x-dX, y-dY, x-dX+width, y-dY+height);
}

