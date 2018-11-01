#include <utility>

#include <utility>

#include <utility>

#include <memory>


#include "Map.h"
#include "Globals.h"
#include "ActionSprite.h"
#include "Util.h"

Map::Map(std::string id, std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap,
        int length, int width, int layers, std::vector<Sprite> sprites, std::vector<Text> texts) {
    this->id = std::move(id);
    this->tilemap = resolveMap(std::move(tileset), std::move(tilemap), length, width, layers);
    this->length = length;
    this->width = width;
    this->layers = layers;
    for (auto spr : sprites) {
        this->sprites.push_back(new Sprite(&spr));
    }
    for (const auto &text : texts) {
        this->texts.push_back(new Text(text));
    }
}

Map *Map::loadMap(std::string filename) {
    std::ifstream is(filename, std::ios::binary);
    if (!is.fail()) {
        cereal::JSONInputArchive inputArchive(is);
        MapJSON loaded = * new MapJSON();
        inputArchive(cereal::make_nvp("mapdata", loaded));
        Util::log("Loading Map " + loaded.id);
        Map *m = new Map(loaded.id, loaded.tileset, loaded.tilemap, loaded.width, loaded.height, loaded.layers, loaded.sprites, loaded.texts);
        return m;
    } else {
        Util::log("Error loading map " + filename + " (File Not Found)", ERROR);
    }
    return nullptr;
}

void Map::test() {
    {
        std::ofstream os("test.json", std::ios::binary);
        cereal::JSONOutputArchive archive(os);
        std::unique_ptr<MapJSON> myData =
                std::make_unique<MapJSON>(
                        new MapJSON(1, "map_test", 2, 16, 16,
                                {{{}}},
                                {"resources/tile00.png", "resources/tile01.png", "resources/tile02.png"},
                                {* new Sprite(64, 64, "anim_sprite",
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
                                 }, 4),
                                 * new Sprite(0,0,"s_hat","resources/hat.png")},
                                {* new Text("Test text tests text when test text tests texts.", 0, 0, "font16", 0xff, 0xff, 0xff)}));
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

std::vector<std::vector<std::vector<Tile *>>> Map::resolveMap(std::vector<std::string> tileset,
        std::vector<std::vector<std::vector<int>>> tilemap, int length, int height, int layers) {

    std::vector<std::vector<std::vector<Tile *>>> resolved;
    resolved.resize((unsigned long)layers);
    for (int l = 0; l < layers; l++) {
        resolved[l].resize((unsigned long)length);
        for (int x = 0; x < length; x++) {
            resolved[l][x].resize((unsigned long)height);
            for (int y = 0; y < height; y++) {
                if (tilemap[l][x][y] == -1) {
                    resolved[l][x][y] = new Tile();
                } else {
                    resolved[l][x][y] = new Tile(tileset[tilemap[l][x][y]]);
                }
                resolved[l][x][y]->setX(x * resolved[l][x][y]->width);
                resolved[l][x][y]->setY(y * resolved[l][x][y]->height);
            }
        }
    }
    return resolved;
}


void Map::draw() {
    for (int l = 0; l < layers; l++) {
        for (int x = 0; x < length; x++) {
            for (int y = 0; y < width; y++) {
                if (tilemap[l][x][y]->frames.empty()) continue;
                tilemap[l][x][y]->draw();
            }
        }
    }
    for (auto text : texts) {
        al_draw_text(fontMap.at(text->font), al_map_rgb(text->r, text->g, text->b), text->x, text->y, 0, text->text.c_str());
    }
    for (auto *spr : sprites) {
        spr->draw();
    }
}

bool Map::checkCollision(Sprite *sprite) {
    BoundingBox *box = sprite->boundingBox;
    for (int l = 0; l < layers; l++) {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < length; y++) {
                Tile *tile = tilemap[l][x][y];
                if (tile->collision == NONE) {
                    continue;
                }
                BoundingBox *check = tile->boundingBox;
                if (check->intersect(box)) {
                    return true;
                }
            }
        }
    }
    return false;
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