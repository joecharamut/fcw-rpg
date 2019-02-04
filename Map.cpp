#include <utility>

#include <iostream>
#include <memory>
#include <experimental/filesystem>
#include <algorithm>

#include "Map.h"
#include "Util.h"
#include "Main.h"
#include "Log.h"
#include "Engine.h"

static std::map<std::string, std::string> mapList = {};

Map::Map(std::string id, std::string defaultRoom, std::vector<std::string> roomFiles,
         std::map<std::string, Text> textsString, std::map<std::string, std::string> soundEffectsString,
         std::map<std::string, std::string> musicString) {
    this->id = std::move(id);
    this->defaultRoom = defaultRoom;
    for (const auto &roomStr : roomFiles) {
        std::ifstream inStream(getFilePath(roomStr, this), std::ios::binary);
        if (!inStream.fail()) {
            cereal::JSONInputArchive input(inStream);
            RoomJSON roomJSON;
            input(cereal::make_nvp("mapdata", roomJSON));
            this->rooms[roomJSON.id] = new Room(roomJSON.id, roomJSON.tileset, roomJSON.tilemap, roomJSON.sprites, roomJSON.events, this);
        }
    }
    this->current_room = rooms[defaultRoom];

    // TODO: Have texts hidden until event trigger?
    for (const auto &text : textsString) {
        this->texts[text.first] = new Text(text.second);
    }

    for (const auto &sfx : soundEffectsString) {
        this->soundEffects[sfx.first] = al_create_sample_instance(Engine::loadSample(Map::getFilePath(sfx.second, this).c_str()));
    }

    for (const auto &str : musicString) {
        this->music[str.first] = al_create_sample_instance(Engine::loadSample(Map::getFilePath(str.second, this).c_str()));
    }
}

// Load map by id
Map* Map::loadMap(std::string id) {
    if (mapList.empty()) enumerateMaps();
    return loadMapFile(mapList[id]);
}

// Load map by file
Map* Map::loadMapFile(std::string filename) {
    // Get system time to calculate load time
    long long int start = Util::getMilliTime();
    // Get the file as an input stream
    std::ifstream is(filename, std::ios::binary);
    // If the stream opened successfully
    if (!is.fail()) {
        // Load the file as JSON input
        cereal::JSONInputArchive inputArchive(is);
        // MapJSON object
        MapJSON loaded;
        // Load the data to the object
        inputArchive(cereal::make_nvp("mapdata", loaded));
        // Create the map with the loaded data
        Map *m = new Map(loaded.id, loaded.defaultRoom, loaded.roomFiles, loaded.textsString,
                loaded.soundEffectsString, loaded.musicString);
        // Get end time
        long long int end = Util::getMilliTime();
        // Print delta
        Log::info("Loaded Map " + loaded.id + " (" + std::to_string(end-start) + " ms)");
        // Return loaded map
        return m;
    }
    // If it didn't open, send error and return null
    Log::error("Error loading map " + filename + " (File Not Found)");
    return nullptr;
}

// Get the path for a map file
std::string Map::getFilePath(std::string filename, Map *map) {
    // Get the base path
    std::string path = mapList[map->id];
    std::vector<std::string> split = Util::splitString(path, "/");
    // Remove map filename from path and add the new file
    path = path.substr(0, path.length() - split.back().length()) + "data/" + filename;
    return path;
}

// Load a list of all maps
std::vector<std::string> Map::enumerateMaps() {
    // Create the list
    std::vector<std::string> maps = {};
    // Define the base path
    std::string path = "resources/maps";
    // If the path doesn't exist, return empty list
    if (!std::experimental::filesystem::exists(path)) {
        return maps;
    }
    // For each folder in the maps directory,
    for (const auto &p : std::experimental::filesystem::directory_iterator(path)) {
        // and each file in that folder,
        for (const auto &p2 : std::experimental::filesystem::directory_iterator(p.path().string())) {
            // Get the filename/path
            std::string file = p2.path().string();
            // Fix Windows being mega gay (Replace \ with /)
            std::replace(file.begin(), file.end(), '\\', '/');
            // If it ends in .json
            if (file.substr(file.length() - 5) == ".json") {
                // Load it and store its id and path
                std::ifstream is(file, std::ios::binary);
                cereal::JSONInputArchive inputArchive(is);
                MapJSON loaded = {};
                inputArchive(cereal::make_nvp("mapdata", loaded));
                maps.push_back(loaded.id);
                mapList[loaded.id] = file;
            }
        }
    }
    // Return the maps list
    return maps;
}

std::vector<Sprite *> Map::getSprites() {
    return current_room->sprites;
}

Sprite* Map::getSpriteById(std::string id) {
    return current_room->getSpriteById(std::move(id));
}

Sprite* Map::checkCollision(Sprite *sprite) {
    return current_room->checkCollision(sprite);
}

void Map::updateViewport(Sprite *spr, bool override) {
    current_room->updateViewport(spr, override);
}

void Map::draw() {
    current_room->draw();

    // For the texts, draw them
    // TODO: Make this event controlled. Maybe something similar to music module, but scrolling text and playing a sound
    // TODO: Fix this / replace with text boxes
    /*for (const auto &textMap : texts) {
        auto text = textMap.second;
        al_draw_text(Engine::fonts[text->font], al_map_rgb(text->r, text->g, text->b), text->x, text->y, 0,
                text->text.c_str());
    }*/
}

void Map::handleEvent(ALLEGRO_EVENT event) {
    if (handlerFunction) {
        handlerFunction(event);
    }
}

void Map::setEventHandlerFunction(void (*handler)(ALLEGRO_EVENT)) {
    this->handlerFunction = handler;
}

void Map::setRoom(std::string roomId) {
    if (!rooms.count(roomId)) {
        return;
    }

    current_room = rooms[roomId];
}
