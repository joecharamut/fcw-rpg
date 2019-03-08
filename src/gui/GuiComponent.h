#ifndef FCWRPG_GUICOMPONENT_H
#define FCWRPG_GUICOMPONENT_H

class GuiComponent {
public:
    const bool interactable = false;

    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;

    virtual void draw() = 0;
    virtual void setPosition(int x, int y) {
        this->x = x;
        this->y = y;
    };
};


#endif //FCWRPG_GUICOMPONENT_H
