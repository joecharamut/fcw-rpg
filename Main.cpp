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
    MapLoader::loadMaps();
    // Load the test map
    Engine::current_map = MapLoader::getMap("map_test");//Map::loadMap("map_test");
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
    struct archive *archive;
    struct archive_entry *entry;
    int r;

    archive = archive_read_new();
    archive_read_support_format_zip(archive);
    r = archive_read_open_filename(archive, "resources/pack_test.map", 0);

    if (r != ARCHIVE_OK) return;

    bool resourcesFileLoaded = false;
    std::vector<std::string> resourceLocations;
    std::vector<std::string> resourcePaths;

    while (archive_read_next_header(archive, &entry) == ARCHIVE_OK) {
        auto entry_length = (size_t) archive_entry_size(entry);
        if (entry_length != 0) {
            std::string path = std::string(archive_entry_pathname(entry));
            if (!resourcesFileLoaded && path.find("resources.json") != std::string::npos) {
                // Read file
                byte *buf = (byte *) calloc(sizeof(byte), entry_length);
                int length = (int) archive_read_data(archive, buf, entry_length);
                // Check for errors
                if (length == ARCHIVE_WARN) {
                    printf("%s\n\n", archive_error_string(archive));
                } else if (length == ARCHIVE_FATAL) {
                    printf("%s\n\n", archive_error_string(archive));
                    return;
                } else if (length == 0) {
                    printf("EOF\n");
                    return;
                }
                // Convert buffer to a StringStream
                std::stringstream ss;
                for (int i = 0; i < length; i++) {
                    ss << buf[i];
                }
                // Decode stream JSON data
                cereal::JSONInputArchive inputArchive(ss);
                ResourceJSON resourceJSON;
                inputArchive(cereal::make_nvp("data", resourceJSON));

                // Store requested resources
                for (auto &res : resourceJSON.resources) {
                    resourceLocations.push_back(res.location);
                    resourcePaths.push_back(res.path);
                }

                resourcesFileLoaded = true;

                archive_read_free(archive);

                archive = archive_read_new();
                archive_read_support_format_zip(archive);
                r = archive_read_open_filename(archive, "resources/pack_test.map", 0);
                if (r != ARCHIVE_OK) return;

                free(buf);
            } else if (resourcesFileLoaded) {
                for (int i = 0; i < resourcePaths.size(); i++) {
                    if (path.find(resourcePaths[i]) != std::string::npos) {
                        std::string ext = "." + Util::splitString(resourcePaths[i], ".").back();
                        byte *buf = (byte *) calloc(sizeof(byte), entry_length);
                        int length = (int) archive_read_data(archive, buf, entry_length);
                        ResourceManager::registerResource(new Resource(
                                ResourceLocation(resourceLocations[i]),
                                ResourceType(ext),
                                buf, (size_t) length
                        ));
                    }
                }
            }
        }
    }
    archive_read_free(archive);
    auto *res = ResourceManager::getResource("pack_test:room_0");
    printf("%s: %d\n", res->location.location.c_str(), res->size);
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
    //MapLoader::loadMaps(); return 0;
    //testArchive(); return 0;
    parseArgs(argc, argv);

    ResourceManager::loadFileToResource("resources/icon.png", "sys:icon");

    // Hand off execution to the engine
    Engine::run();
}