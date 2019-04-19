#include <utility>

#include "GuiComponentText.h"
#include <allegro5/drawing.h>
#include "../Engine.h"

GuiComponentText::GuiComponentText(std::string text, int x, int y, int width, int height, std::string fontStr,
        unsigned char r, unsigned char g, unsigned char b) {
    this->x = x;
    this->y = y;
    this->buf_w = width;
    this->buf_h = height;
    this->text = std::move(text);

    this->r = r;
    this->g = g;
    this->b = b;

    if (width < 1) {
        buf_w = Engine::SCREEN_W;
    }
    if (height < 1) {
        buf_h = Engine::SCREEN_H;
    }

    if (fontStr.empty()) {
        font = Engine::fonts["default"];
    } else if (Engine::fonts.count(fontStr) < 1) {
        Log::warnf("Invalid font %s specified.", fontStr.c_str());
        font = Engine::fonts["default"];
    } else {
        font = Engine::fonts.at(fontStr);
    }
    refreshBuffer();
}

ALLEGRO_BITMAP *GuiComponentText::getRendered() {
    return textBuffer;
}

void GuiComponentText::refreshBuffer() {
    textBuffer = al_create_bitmap(buf_w, buf_h);
    ALLEGRO_BITMAP *old = al_get_target_bitmap();

    al_set_target_bitmap(textBuffer);
    al_clear_to_color(al_map_rgba(0,0,0,0));
    al_draw_multiline_text(font, al_map_rgb(r, g, b), 0, 0, buf_w, al_get_font_line_height(font), 0, text.c_str());
    al_set_target_bitmap(old);
}

void GuiComponentText::setText(std::string newText) {
    this->text = newText;
    refreshBuffer();
}
