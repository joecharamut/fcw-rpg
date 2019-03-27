#include "Gui.h"
#include <allegro5/drawing.h>
#include <allegro5/allegro.h>
#include <cmath>

void Gui::draw() {
    for (GuiComponent *component : components) {
        al_draw_bitmap(component->getRendered(), component->x, component->y, 0);
    }
}

void Gui::addComponent(GuiComponent *component) {
    components.push_back(component);
    updateSelectedComponent(0);
}

void Gui::handleKey(GuiKey key) {
    auto selected = getSelectedComponent();
    int which = -1;
    if (key == GUI_UP) {
        if ((which = getComponentRelativeTo(selected->x, selected->y, GUI_UP)) != -1) {
            updateSelectedComponent(which);
        }
    } else if (key == GUI_DOWN) {
        if ((which = getComponentRelativeTo(selected->x, selected->y, GUI_DOWN)) != -1) {
            updateSelectedComponent(which);
        }
    } else if (key == GUI_LEFT) {
        if ((which = getComponentRelativeTo(selected->x, selected->y, GUI_LEFT)) != -1) {
            updateSelectedComponent(which);
        }
    } else if (key == GUI_RIGHT) {
        if ((which = getComponentRelativeTo(selected->x, selected->y, GUI_RIGHT)) != -1) {
            updateSelectedComponent(which);
        }
    } else if (key == GUI_SELECT) {
        selected->activate();
    }
}

double Gui::distance(int x1, int y1, int x2, int y2) {
    double dist = sqrt(pow((x2 - x1),2) + pow(y2 - y1,2));
    return dist;
}

int Gui::getComponentRelativeTo(int x, int y, GuiKey direction) {
    int limit = 2;

    std::vector<std::pair<int, double>> matches;
    auto selectable = getSelectableComponents();
    for (int i = 0; i < (int) selectable.size(); i++) {
        auto c = selectable[i];
        if (direction == GUI_UP) {
            if (c->y < y) {
                matches.emplace_back(i, distance(x, y, c->x, c->y));
            }
        } else if (direction == GUI_DOWN) {
            if (c->y > y) {
                matches.emplace_back(i, distance(x, y, c->x, c->y));
            }
        } else if (direction == GUI_LEFT) {
            if (c->x < x && (y-limit < c->y && c->y < y+limit)) {
                matches.emplace_back(i, distance(x, y, c->x, c->y));
            }
        } else if (direction == GUI_RIGHT) {
            if (c->x > x && (y-limit < c->y && c->y < y+limit)) {
                matches.emplace_back(i, distance(x, y, c->x, c->y));
            }
        }
    }

    if (matches.empty()) return -1;
    std::pair<int, double> closest = matches[0];
    for (auto m : matches) {
        if (m.second < closest.second) {
            closest = m;
        }
    }

    return closest.first;
}

std::vector<GuiComponent *> Gui::getSelectableComponents() {
    std::vector<GuiComponent *> selectable;
    for (auto c : components) {
        if (c->isSelectable()) {
            selectable.push_back(c);
        }
    }
    return selectable;
}

void Gui::updateSelectedComponent(int index) {
    auto selectable = getSelectableComponents();
    if (selectable.empty() || index < 0 || index > (int) selectable.size()) {
        return;
    }

    selected = index;
    for (auto c : selectable) {
        c->setSelected(false);
    }
    selectable[index]->setSelected(true);
}

GuiComponent *Gui::getSelectedComponent() {
    return getSelectableComponents()[selected];
}
