#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <archive/archive.h>
#include <archive/archive_entry.h>

#include "Sprite.h"
#include "Map.h"
#include "Util.h"
#include "Audio.h"
#include "Object.h"
#include "Main.h"
#include "Keyboard.h"
#include "Log.h"
#include "Engine.h"
#include "Options.h"
#include "ResourceManager.h"
#include "MapLoader.h"

bool fs_state = false;
bool fs_flag = false;

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

            if (Keyboard::getKeyState(ALLEGRO_KEY_ALT) && Keyboard::getKeyState(ALLEGRO_KEY_ENTER)) {
                if (!fs_flag) {
                    fs_state = !fs_state;
                    Engine::setFullscreen(fs_state);
                    fs_flag = true;
                }
            } else {
                if (fs_flag) {
                    fs_flag = false;
                }
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
    Engine::current_map = MapLoader::getMap("map_test");
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
        "--debug",
        "--verbose"
};

const std::vector<std::string> argHelpText = {
        "This help text",
        "Enable debug log output",
        "Enable verbose debug logging"
};

void parseArgs(int argc, char *argv[]) {
    std::vector<std::string> args;
    args.reserve(argc);
    for (int i = 1; i < argc; i++) {
        args.emplace_back(argv[i]);
    }

    if (Util::vectorContains(args, std::string("--debug"))) {
        Options::Runtime::debug = true;
    }
    if (Util::vectorContains(args, std::string("--verbose"))) {
        Options::Runtime::verbose = true;
    }

    if (Util::vectorContains(args, std::string("--help"))) {
        std::string helpStr;
        for (int i = 0; i < validArgs.size(); i++) {
            helpStr += " " + validArgs[i] + "\n\t" + argHelpText[i] + "\n\n";
        }
        printf("Command Line Options:\n%s", helpStr.c_str());
        exit(0);
    }

    for (auto arg : args) {
        Log::debug("Arg: " + arg);
        if (!Util::vectorContains(validArgs, arg)) {
            Log::warn("Invalid argument \"" + arg + "\"");
        }
    }
}

void loadSystemResources() {
    ResourceManager::loadFileToResource("resources/icon.png", "sys:icon");
    ResourceManager::loadFileToResource("resources/font/DOSVGA.ttf", "sys:font_8bit");
}

int main(int argc, char *argv[]) {
    parseArgs(argc, argv);
    loadSystemResources();

    // Hand off execution to the engine
    Engine::run();
}