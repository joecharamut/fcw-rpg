#include <utility>

#include <utility>

#include "Sprite.h"
#include <cstdarg>

// Constructor
Sprite::Sprite(float x, float y, std::string id, std::vector<Animation> frames, COLLISION_TYPE collision) {
    // Set values
    this->x = x;
    this->y = y;
    this->velocity = {0, 0};
    this->frames = std::move(frames);
    for (auto &frame : this->frames) {
        frame.loadFrames();
    }
    this->collision = collision;
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

// Get X coordinate
float Sprite::getX() {
    // X plus X displacement
    return x+dX;
}

// Get Y coordinate
float Sprite::getY() {
    // Y plus Y displacement
    return y+dY;
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

void Sprite::setVelocity(Vec2D newVelocity) {
    this->velocity = newVelocity;
}

void Sprite::update(float delta) {
    printf(".");
    if (lastX >= (2<<16)) {
        lastX = x;
    }
    if (lastY >= -(2<<16)) {
        lastY = y;
    }
    if (lastDelta >= (2<<16)) {
        lastDelta = delta;
    }
    printf(".");
    Vec2D nextPos = TCV(Vec2D(x, y), Vec2D(lastX, lastY), delta, lastDelta, velocity);
    printf("updating: [%f -> %f, %f -> %f]\n", x, nextPos.x, y, nextPos.y);

    lastX = x;
    lastY = y;
    lastDelta = delta;

    this->setX(nextPos.x);
    this->setY(nextPos.y);
}

Vec2D Sprite::TCV(Vec2D thisPos, Vec2D lastPos, float thisDelta, float lastDelta, Vec2D acceleration) {
    Vec2D nextPos;
    float nextX, nextY;

    nextX = thisPos.x + (thisPos.x - lastPos.x) * (thisDelta / lastDelta) + (acceleration.x * thisDelta * thisDelta);
    nextY = thisPos.y + (thisPos.y - lastPos.y) * (thisDelta / lastDelta) + (acceleration.y * thisDelta * thisDelta);

    nextPos = Vec2D(nextX, nextY);

    return nextPos;
}

