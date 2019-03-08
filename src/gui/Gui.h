#ifndef FCWRPG_GUI_H
#define FCWRPG_GUI_H

#include <vector>
#include "GuiComponent.h"

class Gui {
public:
    Gui() = default;

    void draw();
    void addComponent(GuiComponent *component);

private:
    std::vector<GuiComponent *> components;
};


#endif //FCWRPG_GUI_H
