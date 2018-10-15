

#ifndef FCWRPG_BASEMENU_H
#define FCWRPG_BASEMENU_H


#include <allegro5/events.h>
#include <allegro5/allegro_font.h>
#include "Sprite.h"

struct LinkedTextSprite {
    const char *text;
    float x;
    float y;
    LinkedTextSprite *next;
};

struct LinkedTextButton {
    const char *text;
    float x;
    float y;
    void *handler;
    LinkedTextButton *next;
};

class BaseMenu {
public:
    LinkedTextSprite *texts;
    LinkedTextButton *buttons;
    LinkedSprite *sprites;

    BaseMenu();
    virtual void handleEvent(ALLEGRO_EVENT event);
    virtual void draw();
    void addText(const char *text, float x, float y);
    void addButton(const char *text, float x, float y, void *handler);
    void addSprite(Sprite *sprite);
};


#endif //FCWRPG_BASEMENU_H
