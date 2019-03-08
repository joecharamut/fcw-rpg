#ifndef FCWRPG_GUICOMPONENTTEXT_H
#define FCWRPG_GUICOMPONENTTEXT_H


#include <allegro5/bitmap.h>
#include <string>
#include "GuiComponent.h"

class GuiComponentText : public GuiComponent {
public:
    GuiComponentText(std::string text, int x, int y, int width, int height, std::string fontStr, int r, int g, int b);
    void draw() override;

private:
    ALLEGRO_BITMAP *textBuffer;
};


#endif //FCWRPG_GUICOMPONENTTEXT_H
