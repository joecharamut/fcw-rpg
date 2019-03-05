#ifndef FCWRPG_GUICOMPONENT_H
#define FCWRPG_GUICOMPONENT_H


class GuiComponent {
public:
    const bool interactable = false;

    int width = 0;
    int height = 0;

    void draw(int x, int y);
};


#endif //FCWRPG_GUICOMPONENT_H
