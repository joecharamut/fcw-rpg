#ifndef FCWRPG_GUICOMPONENTTEXTFIELD_H
#define FCWRPG_GUICOMPONENTTEXTFIELD_H

#include <gui/GuiComponent.h>
#include <gui/GuiComponentText.h>
#include <gui/GuiComponentGraphics.h>
#include <string>

class GuiComponentTextField : public GuiComponent {
public:
    std::string font;
    std::string buffer = "";

    GuiComponentTextField(int x, int y, int width, int height, std::string font);
    ~GuiComponentTextField() override;

    ALLEGRO_BITMAP *getRendered() override;
    bool isSelectable() override { return true; }
    bool doesLock() override { return true; }
    void activate() override;

private:
    bool state = false;

    GuiComponentText *textComponent = nullptr;
    ALLEGRO_BITMAP *defaultBorder = nullptr;
    ALLEGRO_BITMAP *selectedBorder = nullptr;
    ALLEGRO_BITMAP *target = nullptr;

    void createOverlays();
};


#endif //FCWRPG_GUICOMPONENTTEXTFIELD_H
