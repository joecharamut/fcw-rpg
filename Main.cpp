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

bool fs_state = false;
bool fs_flag = false;

float old_time = 0;
float new_time = 0;

void mapEventHandler(ALLEGRO_EVENT event) {
    if (event.type == ALLEGRO_EVENT_TIMER) {
        new_time = Util::getMilliTime();
        auto hat = Engine::current_map->getSpriteById("s_hat");
        if (hat != nullptr) {
            float hat_x = hat->x;
            float hat_y = hat->y;
            Vec2D vel = hat->velocity;
            if (Keyboard::getKeyState(ALLEGRO_KEY_UP) || Keyboard::getKeyState(ALLEGRO_KEY_W)) {
                //hat_y -= 4;
                vel.y = -0.1;
            } else {
                vel.y = 0.0;
            }
            if (Keyboard::getKeyState(ALLEGRO_KEY_DOWN) || Keyboard::getKeyState(ALLEGRO_KEY_S)) {
                //hat_y += 4;
                vel.y = 0.1;
            } else {
                vel.y = 0.0;
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
            //printf("v: [%f, %f]\n", vel.x, vel.y);
            hat->setVelocity(vel);

            Sprite *spr = Engine::current_map->checkCollision(hat);
            if (spr) {
                std::vector<float> fix = BoundingBox::fixCollision(spr->boundingBox, hat->boundingBox);
                hat->setX(fix[0]);
                hat->setY(fix[1]);
            }
            hat->update(new_time - old_time);
            old_time = new_time;
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

    if (Util::vectorContains(args, std::string("--debug"))) {
        Options::Runtime::debug = true;
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

void test() {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    a = archive_read_new();
    archive_read_support_format_zip(a);
    r = archive_read_open_filename(a, "resources/maps/pack_test.map", 0);
    if (r != ARCHIVE_OK) {
        return;
    }
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        auto len = (size_t) archive_entry_size(entry);
        if (len != 0) {
            printf("%s: %d bytes\n", archive_entry_pathname(entry), (int) len);
            auto *buf = malloc(len);
            int ret = (int) archive_read_data(a, buf, len);
            if (ret == ARCHIVE_WARN || ret == ARCHIVE_FATAL || ret == ARCHIVE_RETRY) {
                printf("%s\n\n", archive_error_string(a));
                return;
            } else if (ret == 0) {
                printf("EOF\n");
                return;
            }
            printf("%02x %02x %02x %02x\n", ((unsigned char *) buf)[0], ((unsigned char *) buf)[1],
                   ((unsigned char *) buf)[2], ((unsigned char *) buf)[3]);
            //printf("%c %c %c %c\n", ((unsigned char *)buf)[0], ((unsigned char *)buf)[1], ((unsigned char *)buf)[2], ((unsigned char *)buf)[3]);
            free(buf);
        }
    }
    r = archive_read_free(a);
    if (r != ARCHIVE_OK) {
        return;
    }
}

int main(int argc, char *argv[]) {
    float testf = -4.0;
    double testd = -4.0;
    printf("f: %f / d: %lf\n", testf, testd);

    //test(); return 0;
    parseArgs(argc, argv);

    // Hand off execution to the engine
    Engine::run();
}