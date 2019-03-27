#ifndef FCWRPG_GUI_H
#define FCWRPG_GUI_H

#include <vector>
#include "GuiComponent.h"

enum GuiKey {
    GUI_UP      = 1<<0,
    GUI_DOWN    = 1<<1,
    GUI_LEFT    = 1<<2,
    GUI_RIGHT   = 1<<3,
    GUI_SELECT  = 1<<4
};

class Gui {
public:
    Gui() = default;

    void draw();
    void addComponent(GuiComponent *component);
    void handleKey(GuiKey key);

private:
    std::vector<GuiComponent *> components;
    int selected = 0;

    std::vector<GuiComponent *> getSelectableComponents();
    void updateSelectedComponent(int index);
    GuiComponent *getSelectedComponent();
    int getComponentRelativeTo(int x, int y, GuiKey direction);
    double distance(int x1, int y1, int x2, int y2);
};


#endif //FCWRPG_GUI_H
