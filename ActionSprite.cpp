#include "ActionSprite.h"
#include <allegro5/allegro_image.h>

ActionSprite::ActionSprite(float x, float y, const char *image, const char *id,
        void (*clickAction)(ActionSprite*, ALLEGRO_EVENT event),
        void (*hoverAction)(ActionSprite*, ALLEGRO_EVENT event)) :
        Sprite(x, y, id, image) {
    this->clickAction = clickAction;
    this->hoverAction = hoverAction;
}