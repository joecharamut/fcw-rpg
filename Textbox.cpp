#include <utility>

#include <utility>

#include "Textbox.h"

Textbox::Textbox(float x, float y, std::string text, ALLEGRO_FONT *font,
                 ALLEGRO_COLOR color, Animation background, Animation border) : Sprite(x, y, background) {
    this->x = x;
    this->y = y;
    this->text = std::move(text);
    this->font = font;
    this->color = color;
    this->background = new Sprite(x,y,background);
    this->border = new Sprite(x, y, std::move(border));
}

void Textbox::draw() {
    background->draw();
    border->draw();
    al_draw_multiline_text(font, color, x,y,800,al_get_font_line_height(font),0, text.c_str());
}