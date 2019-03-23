#ifndef FCWRPG_GUICOMPONENTBUTTON_H
#define FCWRPG_GUICOMPONENTBUTTON_H

#include "GuiComponent.h"
#include <string>

class GuiComponentButton : public GuiComponent {
public:
    void (*onClick)() = nullptr;

    GuiComponentButton(std::string buttonText, void (*onClick)());
    ALLEGRO_BITMAP *getRendered() override;
    bool isSelectable() override { return true; }
};


#endif //FCWRPG_GUICOMPONENTBUTTON_H
