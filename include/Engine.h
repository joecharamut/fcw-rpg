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
#include "Registry.h"
#include "ResourceFile.h"
#include "gui/Gui.h"
#include "object/Object.h"

//class Map;

class Engine {
public:
    const static int SCREEN_W = 512;
    const static int SCREEN_H = 512;
    const static int FPS = 60;

    static Object *player;
    static Map *current_map;
    static std::map<std::string, ALLEGRO_FONT *> fonts;

    static Gui *currentGui;

    static void run();
    static void Exit(int code);

    static void openGui(Gui *gui);
    static void closeGui();

    static void updateLoadingProgress(int which, float inc);
    static void setLoadingProgress(int which, float amt);

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

    static bool f_state;
    static bool f_flag;

    static int gui_flags;

    static std::vector<float> progressBars;

    enum state_offset {
            STATE_ALLEGRO_INIT      = 1 << 0,
            STATE_ALLEGRO_FONT      = 1 << 1,
            STATE_ALLEGRO_TTF       = 1 << 2,
            STATE_ALLEGRO_IMAGE     = 1 << 3,
            STATE_ALLEGRO_KEYBOARD  = 1 << 4,
            STATE_ALLEGRO_AUDIO     = 1 << 5,
            STATE_DISPLAY           = 1 << 6,
            STATE_TIMER             = 1 << 7,
            STATE_EVENT_QUEUE       = 1 << 8
    };
    static int load_state;

    static void setFullscreen(bool enable);

    static void handleControls();

    static bool init();
    static void update();
    static void loadFonts();

    static void refreshProgress();
    static void updateProgress(int pos, float amount);

    class FileException : public std::exception {
    private:
        std::string _message;
    public:
        explicit FileException(std::string message) : _message(std::move(message)) {}

        const char *what() const noexcept override {
            return _message.c_str();
        }
    };
};


#endif //FCWRPG_ENGINE_H
