#include "ActionSprite.h"
#include <allegro5/allegro_image.h>

ActionSprite::ActionSprite(float x, float y, ALLEGRO_BITMAP *image, const char *id,
        void (*clickAction)(ActionSprite*, ALLEGRO_EVENT event), void (*hoverAction)(ActionSprite*, ALLEGRO_EVENT event)) : Sprite(x, y, id, image) {
    this->clickAction = clickAction;
    this->hoverAction = hoverAction;
    this->boundingBox = new BoundingBox(x, y, x+al_get_bitmap_width(image), y+al_get_bitmap_height(image));
}

void ActionSprite::setX(float newX) {
    this->x = newX;
    this->boundingBox = new BoundingBox(x, y, x+al_get_bitmap_width(frames[0]), y+al_get_bitmap_height(frames[0]));
}
void ActionSprite::setY(float newY) {
    this->y = newY;
    this->boundingBox = new BoundingBox(x, y, x+al_get_bitmap_width(frames[0]), y+al_get_bitmap_height(frames[0]));
}