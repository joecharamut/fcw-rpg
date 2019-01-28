#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <chrono>

#include "Sprite.h"
#include "Map.h"
#include "Util.h"
#include "Music.h"
#include "Object.h"
#include "Main.h"
#include "Keyboard.h"
#include "Log.h"
#include "Engine.h"
#include "Thread.h"

void mapEventHandler(ALLEGRO_EVENT event) {
    if (event.type == ALLEGRO_EVENT_TIMER) {
        auto hat = Engine::current_map->getSpriteById("s_hat");
        if (hat != nullptr) {
            float hat_x = hat->x;
            float hat_y = hat->y;
            if (Keyboard::getKeyState(ALLEGRO_KEY_UP) || Keyboard::getKeyState(ALLEGRO_KEY_W)) {
                hat_y -= 4;
            }
            if (Keyboard::getKeyState(ALLEGRO_KEY_DOWN) || Keyboard::getKeyState(ALLEGRO_KEY_S)) {
                hat_y += 4;
            }
            if (Keyboard::getKeyState(ALLEGRO_KEY_LEFT) || Keyboard::getKeyState(ALLEGRO_KEY_A)) {
                hat_x -= 4;
            }
            if (Keyboard::getKeyState(ALLEGRO_KEY_RIGHT) || Keyboard::getKeyState(ALLEGRO_KEY_D)) {
                hat_x += 4;
            }

            hat->setX(hat_x);
            hat->setY(hat_y);

            Sprite *spr = Engine::current_map->checkCollision(hat);
            if (spr) {
                std::vector<float> fix = BoundingBox::fixCollision(spr->boundingBox, hat->boundingBox);
                hat->setX(fix[0]);
                hat->setY(fix[1]);
            }
        }
    }
}

// Function for testing features and stuff
void Main::testing() {
    // Load the test map
    Engine::current_map = Map::loadMap("map_test");
    // Set the event handler TODO: Replace with events from map file, maybe pass events from game to map
    Engine::current_map->setEventHandlerFunction(mapEventHandler);

    // Load in some test music
    ALLEGRO_SAMPLE_INSTANCE *music = Engine::current_map->music.at("mus_cave");
    al_set_sample_instance_playmode(music, ALLEGRO_PLAYMODE_LOOP);
    //Music::playMusic(music);

    // Set the hat position and click action
    auto hat = Engine::current_map->getSpriteById("s_hat");
    hat->setX(Engine::SCREEN_W/4.0f -(hat->width/2.0f));
    hat->setY(Engine::SCREEN_H/4.0f -(hat->height/2.0f));

    Engine::player = hat;
}

const std::vector<std::string> validArgs = {
        "--help",
        "--debug"
};

const std::vector<std::string> argHelpText = {
        "This help text",
        "Enable debug log output"
};

void parseArgs(int argc, char *argv[]) {
    std::vector<std::string> args;
    args.reserve(argc);
    for (int i = 1; i < argc; i++) {
        args.emplace_back(argv[i]);
    }

    for (auto arg : args) {
        Log::debug("Arg: " + arg);
        if (!Util::vectorContains(validArgs, arg)) {
            Log::warn("Invalid argument \"" + arg + "\"");
        }
    }
}

static void test() {
    printf("test\n");
}

static void handle(ThreadMessage t) {
    printf("%s", t.message.c_str());
}

int main(int argc, char *argv[]) {
    parseArgs(argc, argv);

    Thread *thread = new Thread(test, handle);
    thread->postMessage(ThreadMessage("testMessage\n"));
    thread->join();
    return 0;

    // Hand off execution to the engine
    Engine::run();
}