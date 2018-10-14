#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <string>

#include "sprite.h"

#ifndef FCWRPG_TEXTBOX_H
#define FCWRPG_TEXTBOX_H


class Textbox : public Sprite {
public:
    float x;
    float y;
    const char *text;
    ALLEGRO_FONT *font;
    ALLEGRO_COLOR color{};

    Sprite *background;
    Sprite *border;
    Textbox(float x, float y, const char *text, ALLEGRO_FONT *font,
            ALLEGRO_COLOR color, ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *border);

    void draw() override;
};


#endif //FCWRPG_TEXTBOX_H
