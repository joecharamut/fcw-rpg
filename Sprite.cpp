#include "Sprite.h"

Sprite::Sprite(float x, float y, ALLEGRO_BITMAP *image, const char *id) {
    this->x = x;
    this->y = y;
    this->image = image;
    this->width = al_get_bitmap_width(image);
    this->height = al_get_bitmap_height(image);
    this->id = id;
}

Sprite::Sprite(float x, float y, ALLEGRO_BITMAP *image) {
    this->x = x;
    this->y = y;
    this->image = image;
    this->width = al_get_bitmap_width(image);
    this->height = al_get_bitmap_height(image);
    this->id = "s_default_id";
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
