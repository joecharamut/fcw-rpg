#include "Gui.h"

void Gui::draw() {
    for (GuiComponent *component : components) {
        component->draw();
    }
}

void Gui::addComponent(GuiComponent *component) {
    components.push_back(component);
}
