#include "Textbox.h"

Textbox::Textbox(float x, float y, const char *text, ALLEGRO_FONT *font,
                 ALLEGRO_COLOR color, const char *background, const char *border) : Sprite(x, y, background) {
    this->x = x;
    this->y = y;
    this->text = text;
    this->font = font;
    this->color = color;
    this->background = new Sprite(x,y,background);
    if (border) {
        this->border = new Sprite(x, y, border);
    } else {
        this->border = nullptr;
    }
}

void Textbox::draw() {
    background->draw();
    if (border) {
        border->draw();
    }
    al_draw_multiline_text(font, color, x,y,800,al_get_font_line_height(font),0, text);
}