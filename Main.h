#include <allegro5/allegro_font.h>
#include <string>
#include <map>

#ifndef FCWRPG_MAIN_H
#define FCWRPG_MAIN_H

class Main {
public:
    static ALLEGRO_FONT *font8;
    static ALLEGRO_FONT *font16;
    static ALLEGRO_FONT *font24;
    static ALLEGRO_FONT *font32;
    static bool done;
    static int frameCounter;
    static std::map<std::string, ALLEGRO_FONT *> fontMap;
    const static int SCREEN_W = 512;
    const static int SCREEN_H = 512;

    static void update();
    static void loadFonts();
    static int initialize();
};



#endif //FCWRPG_MAIN_H
