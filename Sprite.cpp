#include "Sprite.h"
#include "Globals.h"
#include <cstdarg>

Sprite::Sprite(float x, float y, const char *id, ALLEGRO_BITMAP *image) {
    this->x = x;
    this->y = y;
    this->frames = new LinkedImage();
    this->frames->image = image;
    this->width = al_get_bitmap_width(image);
    this->height = al_get_bitmap_height(image);
    this->id = id;
    this->numFrames = 1;
    this->speed = 1;
}

Sprite::Sprite(float x, float y, const char *id, int frameCount, ...) {

}

void Sprite::draw() {
    LinkedImage *next = frames;
    while (next) {
        if (frameCounter%speed == 0) {
            ++currentFrame %= numFrames;
            while (next != nullptr && next->frameNum != currentFrame) {
                next = next->next;
            }
            if (next != nullptr) {
                al_draw_bitmap(next->image, x, y, 0);
            }
        } else {
            next = next->next;
        }
    }
}

void Sprite::setX(float newX) {
    this->x = newX;
}
void Sprite::setY(float newY) {
    this->y = newY;
}

void Sprite::addFrame(ALLEGRO_BITMAP *image) {
    LinkedImage *newFrame = new LinkedImage();
    newFrame->image = image;
    newFrame->frameNum = frames->frameNum+1;
    numFrames++;
    frames->next = newFrame;
}

