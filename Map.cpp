#include <utility>

#include <utility>

#include <memory>


#include "Map.h"
#include "Globals.h"
#include "ActionSprite.h"

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
    for (auto text : texts) {
        this->texts.push_back(new Text(text));
    }
}

Map *Map::loadMap(std::string filename) {
    std::ifstream is(filename, std::ios::binary);
    if (!is.fail()) {
        cereal::JSONInputArchive inputArchive(is);
        std::unique_ptr<MapJSON> loaded{nullptr};
        inputArchive(cereal::make_nvp("mapdata", loaded));
        printf("Loading Map %s", loaded->id.c_str());
        Map *m = new Map(loaded->id, loaded->tileset, loaded->tilemap, loaded->width, loaded->height, loaded->layers, loaded->sprites, loaded->texts);
        return m;
    } else {
        printf("Error loading map %s", filename.c_str());
    }
    return nullptr;
}

void Map::test() {
    {
        std::ofstream os("test.json", std::ios::binary);
        cereal::JSONOutputArchive archive(os);
        std::unique_ptr<MapJSON> myData =
                std::make_unique<MapJSON>(
                        new MapJSON(1, "map_test", 1, 16, 16,
                                {{{}}},
                                {"resources/tile00.png"},
                                {* new Sprite(0, 0, "test_spr", "resources/tile01.png"),
                                 * new Sprite(64, 64, "anim_sprite",
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
                                 }, 2)},
                                {* new Text("I am test text", 16, 0, "font24", 0xff, 0xff, 0xff)}));
        myData->tilemap.resize(1);
        for (int l = 0; l < 1; l++) {
            myData->tilemap[l].resize(16);
            for (int h = 0; h < 16; h++) {
                myData->tilemap[l][h].resize(16);
                for (int w = 0; w < 16; w++) {
                    myData->tilemap[l][h][w] = 0;
                }
            }
        }
        archive(cereal::make_nvp("mapdata", myData));
    }
}

std::vector<std::vector<std::vector<Tile *>>> Map::resolveMap(std::vector<std::string> tileset,
        std::vector<std::vector<std::vector<int>>> tilemap, int length, int height, int layers) {

    std::vector<std::vector<std::vector<Tile *>>> resolved;
    resolved.resize(layers);
    for (int l = 0; l < layers; l++) {
        resolved[l].resize(length);
        for (int x = 0; x < length; x++) {
            resolved[l][x].resize(height);
            for (int y = 0; y < height; y++) {
                resolved[l][x][y] = new Tile(tileset[tilemap[l][x][y]]);
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
    Text *add = new Text();
    add->text = std::move(text);
    add->x = x;
    add->y = y;
    add->font = font;
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