#include <allegro5/allegro_font.h>
#include <string>
#include <map>

#ifndef FCWRPG_MAIN_H
#define FCWRPG_MAIN_H

const static int SCREEN_W = 512;
const static int SCREEN_H = 512;
const static float FPS = 60;

class Main {
public:
    static bool done;
    static std::map<std::string, ALLEGRO_FONT *> fonts;
    static std::vector<Event *> globalEvents;
    static Map *current_map;

    static void update();
    static bool initGlobalEvents();
    static void executeGlobalEvents();
    static void loadFonts();
    static int init();
};



#endif //FCWRPG_MAIN_H
