#ifndef FCWRPG_ENGINE_H
#define FCWRPG_ENGINE_H

#include <utility>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <exception>
#include <string>
#include <allegro5/allegro_audio.h>
#include <atomic>
#include <thread>

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

    static void run();
    static void exit(int code);

    static void setFullscreen(bool enable);

    static ALLEGRO_BITMAP *loadImage(const char *file);
    static ALLEGRO_SAMPLE *loadSample(const char *file);
    static ALLEGRO_FONT *loadFont(const char *file, int size);


private:
    static bool redraw;
    static std::atomic<bool> done;
    static ALLEGRO_TIMER *timer;
    static ALLEGRO_EVENT_QUEUE *eventQueue;
    static ALLEGRO_DISPLAY *display;
    static double oldTime, newTime, delayTime, fps;

    static int f_scale_w;
    static int f_scale_h;
    static int f_pos_w;
    static int f_pos_h;

    enum state_offset {
            STATE_ALLEGRO_INIT,
            STATE_ALLEGRO_FONT,
            STATE_ALLEGRO_TTF,
            STATE_ALLEGRO_IMAGE,
            STATE_ALLEGRO_KEYBOARD,
            STATE_ALLEGRO_AUDIO,
            STATE_DISPLAY,
            STATE_TIMER,
            STATE_EVENT_QUEUE
    };
    static int state;

    static std::thread renderThread;
    static std::thread consoleThread;
    static std::thread eventThread;

    static bool init();
    static void update();
    static void loadFonts();
    static void renderThreadFunction();
    static void commandThreadFunction();
    static void eventThreadFunction();

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
