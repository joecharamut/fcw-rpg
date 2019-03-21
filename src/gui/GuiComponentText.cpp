#include "GuiComponentText.h"
#include "../Engine.h"

GuiComponentText::GuiComponentText(std::string text, int x, int y, int width, int height, std::string fontStr, int r, int g, int b) {
    this->x = x;
    this->y = y;
    int buf_w = width;
    int buf_h = height;
    ALLEGRO_FONT *font;

    if (width < 1) {
        buf_w = Engine::SCREEN_W;
    }
    if (height < 1) {
        buf_h = Engine::SCREEN_H;
    }
    textBuffer = al_create_bitmap(buf_w, buf_h);

    if (fontStr.empty()) {
        font = Engine::fonts["default"];
    } else if (Engine::fonts.count(fontStr) < 1) {
        Log::warnf("Invalid font %s specified.", fontStr.c_str());
        font = Engine::fonts["default"];
    } else {
        font = Engine::fonts.at(fontStr);
    }

    ALLEGRO_BITMAP *old = al_get_target_bitmap();
    al_set_target_bitmap(textBuffer);
    al_draw_multiline_text(font, al_map_rgb(r, g, b), 0, 0, buf_w, al_get_font_line_height(font), 0, text.c_str());
    al_set_target_bitmap(old);
}

ALLEGRO_BITMAP *GuiComponentText::getRendered() {
    return textBuffer;
}