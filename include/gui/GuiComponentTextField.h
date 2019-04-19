#ifndef FCWRPG_GUICOMPONENTTEXTFIELD_H
#define FCWRPG_GUICOMPONENTTEXTFIELD_H

#include "GuiComponent.h"
#include "GuiComponentText.h"
#include "GuiComponentGraphics.h"
#include <string>

class GuiComponentTextField : public GuiComponent {
public:
    std::string font;
    std::string buffer = "";

    GuiComponentTextField(int x, int y, int width, int height, std::string font);

    ALLEGRO_BITMAP *getRendered() override;
    bool isSelectable() override { return true; }
    bool doesLock() override { return true; }
    void activate() override;

private:
    bool state = false;

    GuiComponentText *textComponent;
    ALLEGRO_BITMAP *defaultBorder;
    ALLEGRO_BITMAP *selectedBorder;
    ALLEGRO_BITMAP *target;

    void createOverlays();
};


#endif //FCWRPG_GUICOMPONENTTEXTFIELD_H
