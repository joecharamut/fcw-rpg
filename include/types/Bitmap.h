#ifndef FCWRPG_BITMAP_H
#define FCWRPG_BITMAP_H

#include <allegro5/bitmap.h>
#include <vector>

class Bitmap {
public:
    static std::vector<Bitmap *> bitmaps;

    int width = 0;
    int height = 0;

    explicit Bitmap(ALLEGRO_BITMAP *bmp);
    Bitmap(int w, int h);
    void destroy();
    ALLEGRO_BITMAP *get();

private:
    ALLEGRO_BITMAP *bmp = nullptr;
};



#endif //FCWRPG_BITMAP_H
