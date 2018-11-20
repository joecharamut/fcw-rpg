#include <utility>
#include <iostream>
#include <memory>
#include <experimental/filesystem>

#include "Map.h"
#include "Globals.h"
#include "Util.h"

static bool loaded = false;
static std::map<std::string, std::string> mapList = {};

Map::Map(std::string id, std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap,
        std::vector<Sprite> sprites, std::vector<Text> texts, std::vector<std::string> music) {
    this->id = std::move(id);

    for (auto &str : tileset) {
        str = Map::getFilePath(str);
    }

    resolveMap(std::move(tileset), std::move(tilemap));

    for (auto spr : sprites) {
        for (auto &animation : spr.frames) {
            for (auto &frame : animation.frames) {
                frame = Map::getFilePath(frame);
            }
        }
        this->sprites.push_back(new Sprite(&spr));
    }

    for (const auto &text : texts) {
        this->texts.push_back(new Text(text));
    }

    for (const auto &file : music) {
        this->music.push_back(al_load_sample(Map::getFilePath(file).c_str()));
    }
}

Map* Map::loadMap(std::string mapname) {
    if (!loaded) enumerateMaps();
    return loadMapFile(mapList[mapname]);
}

Map* Map::loadMapFile(std::string filename) {
    std::ifstream is(filename, std::ios::binary);
    if (!is.fail()) {
        cereal::JSONInputArchive inputArchive(is);
        MapJSON loaded = * new MapJSON();
        inputArchive(cereal::make_nvp("mapdata", loaded));
        Util::log("Loading Map " + loaded.id);
        Map *m = new Map(loaded.id, loaded.tileset, loaded.tilemap, loaded.sprites, loaded.texts, loaded.music);
        return m;
    } else {
        Util::log("Error loading map " + filename + " (File Not Found)", ERROR);
    }
    return nullptr;
}

std::string Map::getFilePath(std::string filename) {
    std::string path = mapList[id];
    path = path.substr(0, path.length() - 8) + "data/" + filename;
    return path;
}

void Map::test() {
    {
        std::ofstream os("test.json", std::ios::binary);
        cereal::JSONOutputArchive archive(os);
        std::unique_ptr<MapJSON> myData =
                std::make_unique<MapJSON>(
                        new MapJSON(1, "map_test",
                                {{{}}},
                                {"resources/tile00.png", "resources/tile01.png", "resources/tile02.png"},
                                {* new Sprite(64, 64, "anim_sprite", *new Animation("IDLE",
                                        std::vector<std::string>{
                                     "resources/rainbow/frame-0.png",
                                     "resources/rainbow/frame-1.png",
                                     "resources/rainbow/frame-2.png",
                                     "resources/rainbow/frame-3.png",
                                     "resources/rainbow/frame-4.png",
                                     "resources/rainbow/frame-5.png",
                                     "resources/rainbow/frame-6.png",
                                     "resources/rainbow/frame-7.png",
                                     "resources/rainbow/frame-8.png",
                                     "resources/rainbow/frame-9.png",
                                     "resources/rainbow/frame-10.png",
                                     "resources/rainbow/frame-11.png"
                                 }, 4)),
                                 * new Sprite(0,0,"s_hat",*new Animation("resources/hat.png"))},
                                {* new Text("Test text tests text when test text tests texts.", 0, 0, "font16", 0xff, 0xff, 0xff)},
                                {}));
        myData->tilemap.resize(2);
        myData->tilemap[0].resize(16);
        for (int h = 0; h < 16; h++) {
            myData->tilemap[0][h].resize(16);
            for (int w = 0; w < 16; w++) {
                myData->tilemap[0][h][w] = 0;
            }
        }
        myData->tilemap[1].resize(16);
        for (int h = 0; h < 16; h++) {
            myData->tilemap[1][h].resize(16);
            for (int w = 0; w < 16; w++) {
                myData->tilemap[1][h][w] = (h%3 != 0 ? (h%3) : -1);
            }
        }
        archive(cereal::make_nvp("mapdata", *myData));
    }
}

std::vector<std::string> Map::enumerateMaps() {
    std::vector<std::string> maps = {};
    std::string path = "resources/maps";
    if (!std::experimental::filesystem::exists(path)) {
        return maps;
    }
    for (const auto &p : std::experimental::filesystem::directory_iterator(path)) {
        for (const auto &p2 : std::experimental::filesystem::directory_iterator(p.path().string())) {
            std::string file = p2.path().string();
            if (file.substr(file.length() - 5) == ".json") {
                std::ifstream is(file, std::ios::binary);
                cereal::JSONInputArchive inputArchive(is);
                MapJSON loaded = {};
                inputArchive(cereal::make_nvp("mapdata", loaded));
                maps.push_back(loaded.id);
                mapList[loaded.id] = file;
            }
        }
    }
    loaded = true;
    return maps;
}

std::vector<Animation *> resolveTileset(std::vector<std::string> in) {
    std::vector<Animation *> out = {};
    out.reserve(in.size());
    for (const auto &frame : in) {
        out.push_back(new Animation(frame));
    }
    return out;
}

void Map::resolveMap(std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap) {
    int layers = tilemap.size();
    int length = tilemap[0].size();
    int width = tilemap[0][0].size();

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
        al_draw_text(fontMap.at(text->font), al_map_rgb(text->r, text->g, text->b), text->x, text->y, 0, text->text.c_str());
    }
    for (auto *spr : sprites) {
        spr->draw();
    }
}

void Map::updateViewport(Sprite *spr, bool override) {
    bool changed = false;

    float cX = SCREEN_W/2.0f;
    float cY = SCREEN_H/2.0f;
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
            if (newX >= 0 && newX <= al_get_bitmap_width(backgrounds[0])-SCREEN_W) {
                viewportX -= dX;
                spr->setX(spr->x+dX);
                changed = true;
            }
            if (newY >= 0 && newY <= al_get_bitmap_height(backgrounds[0])-SCREEN_H) {
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
    /*for (int l = 0; l < layers; l++) {
        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                Tile *tile = tilemap[l][x][y];
                if (tile->collision == NONE) {
                    continue;
                }
                BoundingBox *check = tile->boundingBox;
                if (BoundingBox::intersect(check, box)) {
                    return tile;
                }
            }
        }
    }*/
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
