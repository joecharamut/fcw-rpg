#include <utility>
#include <iostream>
#include <memory>
#include <experimental/filesystem>
#include <chrono>

#include "Map.h"
#include "Util.h"
#include "Main.h"

static std::map<std::string, std::string> mapList = {};

// Constructor
Map::Map(std::string id, std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap,
        std::vector<Sprite> sprites, std::vector<std::string> events, std::vector<Text> texts,
        std::vector<std::string> music) {
    // Copy in id
    this->id = std::move(id);

    // Copy in tileset, with path
    for (auto &str : tileset) {
        str = Map::getFilePath(str);
    }

    // Load the tilemap as actual images into memory
    resolveMap(std::move(tileset), std::move(tilemap));

    // Load in the sprites
    for (auto spr : sprites) {
        for (auto &animation : spr.frames) {
            for (auto &frame : animation.frames) {
                frame = Map::getFilePath(frame);
            }
        }
        this->sprites.push_back(new Sprite(&spr));
    }

    // Load in events
    for (const auto &eventStr : events) {
        this->events.push_back(Event::decode(eventStr));
    }

    // Load in Texts
    // TODO: Have texts hidden until event trigger?
    for (const auto &text : texts) {
        this->texts.push_back(new Text(text));
    }

    // Load in music
    for (const auto &file : music) {
        this->music.push_back(al_load_sample(Map::getFilePath(file).c_str()));
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
        Map *m = new Map(loaded.id, loaded.tileset, loaded.tilemap, loaded.sprites, loaded.events, loaded.texts, loaded.music);
        // Get end time
        long long int end = Util::getMilliTime();
        // Print delta
        Util::log("Loaded Map " + loaded.id + " (" + std::to_string(end-start) + " ms)");
        // Return loaded map
        return m;
    }
    // If it didn't open, send error and return null
    Util::log("Error loading map " + filename + " (File Not Found)", ERROR);
    return nullptr;
}

// Get the path for a map file
std::string Map::getFilePath(std::string filename) {
    // Get the base path
    std::string path = * new std::string(mapList[id]);
    // Remove map filename from path and add the new file
    path = path.substr(0, path.length() - Util::splitString(path, "/").back().length()) + "data/" + filename;
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

// Function to resolve the tileset to a list of Animations with the frames loaded in memory
std::vector<Animation *> resolveTileset(std::vector<std::string> in) {
    // Create out array
    std::vector<Animation *> out = {};
    // Reserve space
    out.reserve(in.size());
    // For each tile, create an Animation and store it
    for (const auto &tile : in) {
        out.push_back(new Animation(tile));
    }
    return out;
}

// Function to resolve the tileset and tilemap into images
void Map::resolveMap(std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap) {
    // Get layers, width, and height
    int layers = (int) tilemap.size();
    int length = (int) tilemap[0].size();
    int width  = (int) tilemap[0][0].size();

    std::vector<Animation *> animationMap = resolveTileset(std::move(tileset));
    for (int l = 0; l < layers; l++) {
        backgrounds.push_back(al_create_bitmap(length*32, width*32));
        al_set_target_bitmap(backgrounds[l]);
        al_clear_to_color(al_map_rgba(0x00, 0x00, 0x00, 0x00));
        for (int x = 0; x < length; x++) {
            for (int y = 0; y < width; y++) {
                if (tilemap[l][x][y] != -1) {
                    al_draw_bitmap(animationMap[tilemap[l][x][y]]->nextFrame(), x*32, y*32, 0);
                }
            }
        }
    }
    al_set_target_backbuffer(al_get_current_display());
}


void Map::draw() {
    for (auto bg : backgrounds) {
        //al_draw_bitmap_region(bg, viewportX, viewportY, al_get_bitmap_width(bg), al_get_bitmap_height(bg), 0, 0, 0);
        al_draw_bitmap_region(bg, viewportX, viewportY, 512, 512, 0, 0, 0);
    }
    for (auto text : texts) {
        al_draw_text(Main::fonts[text->font], al_map_rgb(text->r, text->g, text->b), text->x, text->y, 0, text->text.c_str());
    }
    for (auto *spr : sprites) {
        spr->draw();
    }
}

void Map::updateViewport(Sprite *spr, bool override) {
    bool changed = false;

    float cX = Main::SCREEN_W/2.0f;
    float cY = Main::SCREEN_H/2.0f;
    float csX = (spr->x + (spr->width/2.0f));
    float csY = (spr->y + (spr->height/2.0f));
    if (csX != cX || csY != cY) {
        float dX = cX - csX;
        float dY = cY - csY;
        if (override) {
            viewportX -= dX;
            viewportY -= dY;
            spr->setX(spr->x+dX);
            spr->setY(spr->y+dY);
            changed = true;
        } else {
            float newX = viewportX - dX;
            float newY = viewportY - dY;
            if (newX >= 0 && newX <= al_get_bitmap_width(backgrounds[0])-Main::SCREEN_W) {
                viewportX -= dX;
                spr->setX(spr->x+dX);
                changed = true;
            }
            if (newY >= 0 && newY <= al_get_bitmap_height(backgrounds[0])-Main::SCREEN_H) {
                viewportY -= dY;
                spr->setY(spr->y+dY);
                changed = true;
            }
        }
        if (changed) {
            for (auto *sprite : sprites) {
                if (sprite->id != spr->id) {
                    sprite->setDisplace(viewportX, viewportY);
                }
            }
        }
    }
}

Sprite* Map::checkCollision(Sprite *sprite) {
    BoundingBox *box = sprite->boundingBox;
    for (auto *spr : sprites) {
        if (spr->collision == NONE) {
            continue;
        }
        BoundingBox *check = spr->boundingBox;
        if (BoundingBox::intersect(check, box)) {
            return spr;
        }
    }
    return nullptr;
}

void Map::addSprite(Sprite *sprite) {
    sprites.push_back(sprite);
}

void Map::addText(std::string text, std::string font, unsigned char r, unsigned char g, unsigned char b, float x, float y) {
    auto *add = new Text();
    add->text = std::move(text);
    add->x = x;
    add->y = y;
    add->font = std::move(font);
    add->r = r;
    add->g = g;
    add->b = b;

    texts.push_back(add);
}

Sprite *Map::getSpriteById(std::string id) {
    for (auto *spr : sprites) {
        if (spr->id == id) {
            return spr;
        }
    }
    return nullptr;
}



void Map::handleEvent(ALLEGRO_EVENT event) {
    if (handlerFunction) {
        handlerFunction(event);
    }
}

void Map::setEventHandlerFunction(void (*handler)(ALLEGRO_EVENT)) {
    this->handlerFunction = handler;
}
