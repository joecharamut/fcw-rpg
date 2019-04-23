#include <gui/GuiComponentButton.h>

GuiComponentButton::GuiComponentButton(std::string buttonText, void (*onClick)(), int x, int y, int width, int height, std::string font) {
    this->x = x;
    this->y = y;
    this->onClick = onClick;

    this->unselectedBuffer = new GuiComponentText(buttonText, x, y, width, height, font, 255, 255, 255);
    this->selectedBuffer = new GuiComponentText(buttonText, x, y, width, height, font, 255, 255, 0);
}

GuiComponentButton::~GuiComponentButton() {
    delete unselectedBuffer;
    delete selectedBuffer;
}

ALLEGRO_BITMAP *GuiComponentButton::getRendered() {
    if (selected) {
        return selectedBuffer->getRendered();
    }
    return unselectedBuffer->getRendered();
}

void GuiComponentButton::activate() {
    onClick();
}
