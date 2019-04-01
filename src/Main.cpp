#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

#include "Sprite.h"
#include "Map.h"
#include "Util.h"
#include "Main.h"
#include "Log.h"
#include "Engine.h"
#include "MapLoader.h"

#include "gui/GuiComponentText.h"
#include "gui/GuiComponentGraphics.h"
#include "gui/GuiComponentButton.h"
#include "gui/GuiComponentTextField.h"

void mapEventHandler(ALLEGRO_EVENT event) {}

void button1Handler() { Log::debug("Button1"); }
void closeButton() { Engine::closeGui(); }

// Function for testing features and stuff
void Main::testing() {
    Sprite *player = new Sprite(0, 0, "player_hat", {Animation("map_test:hat")}, COLLISION_NONE);
    Engine::player = player;
    // Load the test map
    //Engine::current_map = MapLoader::getMap("map_test");
    // Set the event handler TODO: Replace with events from map file, maybe pass events from game to map
    //Engine::current_map->setEventHandlerFunction(mapEventHandler);

    Gui *gui = new Gui();
    gui->addComponent(new GuiComponentGraphics(32, 32, 448, 448, 0, 0, 0));
    gui->addComponent(new GuiComponentGraphics(128, 128, 32, 32, 255, 0, 0));
    gui->addComponent(new GuiComponentText("test text test text", 48, 64, 0, 0, "font16", 255,255,255));
    gui->addComponent(new GuiComponentButton("Button 1", button1Handler, 96,  128, 0, 0, "font16"));
    gui->addComponent(new GuiComponentTextField(64, 256, 256, 16, "font16"));
    gui->addComponent(new GuiComponentTextField(64, 280, 256, 16, "font16"));
    gui->addComponent(new GuiComponentTextField(64, 304, 256, 16, "font16"));
    gui->addComponent(new GuiComponentTextField(64, 330, 256, 16, "font16"));
    gui->addComponent(new GuiComponentButton("Close", closeButton, 232, 432, 0, 0, "font16"));

    //Keyboard::setKeyBuffer(true);
    Engine::openGui(gui);

    // Load in some test music
    //ALLEGRO_SAMPLE_INSTANCE *music = Engine::current_map->music.at("mus_cave");
    //al_set_sample_instance_playmode(music, ALLEGRO_PLAYMODE_LOOP);
    //Music::playMusic(music);

    // Set the hat position
    //auto hat = Engine::current_map->getSpriteById("s_hat");
    player->setX(Engine::SCREEN_W/4.0f - (player->width/2.0f));
    player->setY(Engine::SCREEN_H/4.0f - (player->height/2.0f));
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
        for (int i = 0; i < (int)validArgs.size(); i++) {
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
    ResourceFile *icon = ResourceFile::loadFileToResource("resources/icon.png");
    ResourceFile *font = ResourceFile::loadFileToResource("resources/font/DOSVGA.ttf");
    Engine::resourceFileRegistry.put(icon, "sys:icon");
    Engine::resourceFileRegistry.put(font, "sys:font_8bit");
}

int main(int argc, char *argv[]) {
    parseArgs(argc, argv);
    loadSystemResources();

    // Hand off execution to the engine
    Engine::run();
}