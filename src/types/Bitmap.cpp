#include <types/Bitmap.h>

std::vector<Bitmap *> Bitmap::bitmaps;

Bitmap::Bitmap(ALLEGRO_BITMAP *bmp) {
    bitmaps.push_back(this);
    this->bmp = bmp;
    this->width = al_get_bitmap_width(this->bmp);
    this->height = al_get_bitmap_height(this->bmp);
}

Bitmap::Bitmap(int w, int h) {
    this->bmp = al_create_bitmap(w, h);
    this->width = w;
    this->height = h;
}

void Bitmap::destroy() {
    al_destroy_bitmap(bmp);
}

ALLEGRO_BITMAP *Bitmap::get() {
    return bmp;
}
