#include <allegro5/drawing.h>
#include <allegro5/allegro.h>

#include <gui/GuiComponentGraphics.h>

GuiComponentGraphics::GuiComponentGraphics(int x, int y, int width, int height,
        unsigned char r, unsigned char g, unsigned char b) {
    this->x = x;
    this->y = y;

    ALLEGRO_BITMAP *old = al_get_target_bitmap();
    graphicsBuffer = al_create_bitmap(width, height);
    al_set_target_bitmap(graphicsBuffer);

    al_clear_to_color(al_map_rgb(r,g,b));

    al_set_target_bitmap(old);
}

ALLEGRO_BITMAP *GuiComponentGraphics::getRendered() {
    return graphicsBuffer;
}
