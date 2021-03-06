#ifndef FCWRPG_GUICOMPONENTBUTTON_H
#define FCWRPG_GUICOMPONENTBUTTON_H

#include <gui/GuiComponent.h>
#include <gui/GuiComponentText.h>
#include <string>

class GuiComponentButton : public GuiComponent {
public:
    void (*onClick)() = nullptr;

    GuiComponentButton(std::string buttonText, void (*onClick)(), int x, int y, int width, int height, std::string font);
    ~GuiComponentButton() override;

    ALLEGRO_BITMAP *getRendered() override;
    bool isSelectable() override { return true; }
    void activate() override;

private:
    GuiComponentText *unselectedBuffer;
    GuiComponentText *selectedBuffer;
};


#endif //FCWRPG_GUICOMPONENTBUTTON_H
