#ifndef FCWRPG_GUICOMPONENTTEXT_H
#define FCWRPG_GUICOMPONENTTEXT_H


#include <allegro5/bitmap.h>
#include <string>
#include "GuiComponent.h"

class GuiComponentText : public GuiComponent {
public:
    GuiComponentText(std::string text, int width, int height, std::string fontStr, int r, int g, int b);
private:
    ALLEGRO_BITMAP *textBuffer;
};


#endif //FCWRPG_GUICOMPONENTTEXT_H
