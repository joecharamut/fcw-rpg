#ifndef FCWRPG_GUI_H
#define FCWRPG_GUI_H

#include <vector>
#include <gui/GuiComponent.h>

enum GuiKey {
    GUI_UP      = 1u<<0u,
    GUI_DOWN    = 1u<<1u,
    GUI_LEFT    = 1u<<2u,
    GUI_RIGHT   = 1u<<3u,
    GUI_SELECT  = 1u<<4u
};

class Gui {
public:
    Gui() = default;
    void unload();

    void draw();
    void addComponent(GuiComponent *component);
    void handleKey(GuiKey key);

private:
    std::vector<GuiComponent *> components;
    int selected = 0;
    bool lockSelect = false;

    std::vector<GuiComponent *> getSelectableComponents();
    void updateSelectedComponent(int index);
    GuiComponent *getSelectedComponent();
    int getComponentRelativeTo(int x, int y, GuiKey direction);
    double distance(int x1, int y1, int x2, int y2);
};


#endif //FCWRPG_GUI_H
