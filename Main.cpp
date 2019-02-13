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

void testArchive() {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    a = archive_read_new();
    archive_read_support_format_zip(a);
    r = archive_read_open_filename(a, "resources/pack_test.map", 0);
    if (r != ARCHIVE_OK) {
        return;
    }
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        auto len = (size_t) archive_entry_size(entry);
        if (len != 0) {
            std::string path = std::string(archive_entry_pathname(entry));
            if (path.length() > 14 && path.substr(path.length() - 14) == "resources.json") {
                printf("%s: %d bytes\n", archive_entry_pathname(entry), (int) len);
                byte *buf = (byte *) calloc(sizeof(byte), len);
                int length = (int) archive_read_data(a, buf, len);
                if (length == ARCHIVE_WARN || length == ARCHIVE_RETRY) {
                    printf("%s\n\n", archive_error_string(a));
                } else if (length == ARCHIVE_FATAL) {
                    printf("%s\n\n", archive_error_string(a));
                    return;
                } else if (length == 0) {
                    printf("EOF\n");
                    return;
                }

                std::stringstream ss;
                for (int i = 0; i < length; i++) {
                    ss << buf[i];
                }

                cereal::JSONInputArchive inputArchive(ss);
                ResourceJSON resourceJSON;
                inputArchive(cereal::make_nvp("data", resourceJSON));
                for (auto r : resourceJSON.resources) {
                    printf("%s -> %s\n", r.first.c_str(), r.second.c_str());
                }

                free(buf);
            }
        }
    }
    r = archive_read_free(a);
    if (r != ARCHIVE_OK) {
        return;
    }
}

void test() {
    FILE *fp = fopen("resources/maps/map_test/map.json", "r");
    struct stat sbuf;
    fstat(fileno(fp), &sbuf);
    size_t size = (size_t) sbuf.st_size;
    byte *data = (byte *) calloc(sizeof(byte), size);
    fread(data, size, 1, fp);

    printf("size: %d\n\n", (int) size);

    {
        ResourceManager::registerResource(
                new Resource(
                        ResourceLocation("test:testing"),
                        ResourceType("JSON", ".json"),
                        data, size
                )
        );
    }
    {
        Resource *res = ResourceManager::getResource(ResourceLocation("test:testing"));
        printf("found resource\nlocation: %s\ntype: %s\next: %s\nsize: %d\n\n",
                res->location.location.c_str(), res->type.description.c_str(), res->type.extension.c_str(), res->size);
        if (res) {
            byte *buf = (byte *) calloc(sizeof(byte), res->size);
            ALLEGRO_FILE *file = res->openAllegroFile();
            size_t ret = al_fread(file, buf, res->size);
            printf("size: %d\n", ret);
            for (int i = 0; i < res->size; i++) {
                printf("%c", buf[i]);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    testArchive(); return 0;
    parseArgs(argc, argv);

    // Hand off execution to the engine
    Engine::run();
}