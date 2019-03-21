#include "Gui.h"
#include <allegro5/drawing.h>
#include <allegro5/allegro.h>

void Gui::draw() {
    for (GuiComponent *component : components) {
        al_draw_bitmap(component->getRendered(), component->x, component->y, 0);
    }
}

void Gui::addComponent(GuiComponent *component) {
    components.push_back(component);
}
