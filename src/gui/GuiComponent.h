#ifndef FCWRPG_GUICOMPONENT_H
#define FCWRPG_GUICOMPONENT_H

#include <allegro5/bitmap.h>
#include "Gui.h"

class Gui;

class GuiComponent {
public:
    Gui *parent;

    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    bool selected = false;

    //virtual void draw() = 0;
    virtual ALLEGRO_BITMAP *getRendered() = 0;

    virtual bool isSelectable() { return false; }
    virtual void setSelected(bool state) { selected = state; };
    virtual void activate() { ; };

    virtual void setPosition(int x, int y) {
        this->x = x;
        this->y = y;
    };
};


#endif //FCWRPG_GUICOMPONENT_H
