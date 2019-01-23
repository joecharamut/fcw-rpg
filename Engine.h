
#ifndef FCWRPG_ENGINE_H
#define FCWRPG_ENGINE_H

#include <utility>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <exception>
#include <string>
#include <allegro5/allegro_audio.h>
#include <atomic>

#include "Map.h"
class Map;

class Engine {
public:
    const static int SCREEN_W = 512;
    const static int SCREEN_H = 512;
    const static int FPS = 60;

    static Sprite *player;
    static Map *current_map;
    static std::map<std::string, ALLEGRO_FONT *> fonts;

    static int init();
    static void run();

    static ALLEGRO_BITMAP *loadImage(const char *file);
    static ALLEGRO_SAMPLE *loadSample(const char *file);
    static ALLEGRO_FONT *loadFont(const char *file, int size);

private:
    static bool redraw;
    static std::atomic<bool> done;
    static ALLEGRO_EVENT_QUEUE *eventQueue;
    static ALLEGRO_DISPLAY *display;
    static double oldTime, newTime, delayTime, fps;

    static void loadFonts();

    static void update();
    static void renderThread();
    static void commandThread();
    static void eventThread();

    class FileException : public std::exception {
    private:
        std::string message_;
    public:
        explicit FileException(std::string message) : message_(std::move(message)) {}
        virtual const char *what() const throw() {
            return message_.c_str();
        }
    };
};


#endif //FCWRPG_ENGINE_H
