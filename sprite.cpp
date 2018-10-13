#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class sprite {
public:
    float x;
    float y;
    ALLEGRO_BITMAP *image;

    sprite(float x, float y, ALLEGRO_BITMAP *image) {
        this->x = x;
        this->y = y;
        this->image = image;
    }

    void draw() {
        al_draw_bitmap(image, x, y, 0);
    }

    void setX(float newX) {
        this->x = newX;
    }
    void setY(float newY) {
        this->y = newY;
    }
};


