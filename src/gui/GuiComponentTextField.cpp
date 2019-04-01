#include "GuiComponentTextField.h"
#include "../module/Keyboard.h"
#include "../Log.h"

GuiComponentTextField::GuiComponentTextField(int x, int y, int width, int height, std::string font) {
    this->x = x;
    this->y = y;
    this->width = width+4;
    this->height = height+4;
    this->font = font;

    this->textComponent = new GuiComponentText("Placeholder Text", x, y, width, height, font, 255, 255, 255);
    this->target = al_create_bitmap(this->width, this->height);

    createOverlays();
}

void GuiComponentTextField::createOverlays() {
    ALLEGRO_BITMAP *old = al_get_target_bitmap();
    this->defaultBorder = al_create_bitmap(width, height);
    this->selectedBorder = al_create_bitmap(width, height);
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR yellow = al_map_rgb(255, 255, 0);

    al_set_target_bitmap(defaultBorder);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (x < 2 || x > width-2 || y < 2 || y > height-2) {
                al_draw_pixel(x, y, white);
            }
        }
    }

    al_set_target_bitmap(selectedBorder);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (x < 2 || x > width-2 || y < 2 || y > height-2) {
                al_draw_pixel(x, y, yellow);
            }
        }
    }

    al_set_target_bitmap(old);
}

ALLEGRO_BITMAP *GuiComponentTextField::getRendered() {
    if (state) {
        std::pair<char, int> c;
        while ((c = Keyboard::consumeKey()).first != -1) {
            if (c.second == ALLEGRO_KEY_BACKSPACE) {
                if (!buffer.empty()) {
                    buffer.pop_back();
                }
            } else {
                if (isprint(c.first)) {
                    buffer += c.first;
                }
            }
        }
        //Log::debug(buffer);
        textComponent->setText(buffer);
    }
    ALLEGRO_BITMAP *old = al_get_target_bitmap();
    al_set_target_bitmap(target);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_bitmap(textComponent->getRendered(), 2, 2, 0);
    al_draw_bitmap((selected ? selectedBorder : defaultBorder), 0, 0, 0);
    al_set_target_bitmap(old);
    return target;
}

void GuiComponentTextField::activate() {
    if (state) {
        // Unfocus
        Keyboard::setKeyBuffer(false);
    } else {
        // Focus
        Keyboard::setKeyBuffer(true);
    }

    state = !state;
}
