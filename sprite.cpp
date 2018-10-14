#include "sprite.h"

Sprite::Sprite(float x, float y, ALLEGRO_BITMAP *image) {
    this->x = x;
    this->y = y;
    this->image = image;
}

void Sprite::draw() {
    al_draw_bitmap(image, x, y, 0);
}

void Sprite::setX(float newX) {
    this->x = newX;
}
void Sprite::setY(float newY) {
    this->y = newY;
}
