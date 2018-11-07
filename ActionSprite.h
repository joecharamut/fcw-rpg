#ifndef FCWRPG_ACTIONSPRITE_H
#define FCWRPG_ACTIONSPRITE_H


#include "Sprite.h"
#include "BoundingBox.h"

class ActionSprite : public Sprite {
public:
    void (*clickAction)(ActionSprite*, ALLEGRO_EVENT event);
    void (*hoverAction)(ActionSprite*, ALLEGRO_EVENT event);

    ActionSprite(float x, float y, Animation image, const char *id,
            void (*clickAction)(ActionSprite*, ALLEGRO_EVENT event),
            void (*hoverAction)(ActionSprite*, ALLEGRO_EVENT event));
};


#endif //FCWRPG_ACTIONSPRITE_H
