#include <utility>

#include <utility>

#include <memory>


#include "Map.h"
#include "Globals.h"
#include "ActionSprite.h"

Map::Map(std::string id, std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap, int length, int width, int layers) {
    this->id = std::move(id);
    this->tilemap = resolveMap(std::move(tileset), std::move(tilemap), length, width, layers);
    this->length = length;
    this->width = width;
    this->layers = layers;
}

Map *Map::loadMap(std::string filename) {
    std::ifstream is(filename, std::ios::binary);
    if (!is.fail()) {
        cereal::JSONInputArchive inputArchive(is);
        std::unique_ptr<MapJSON> loaded{nullptr};
        inputArchive(cereal::make_nvp("mapdata", loaded));
        printf("Loading Map %s", loaded->id.c_str());
        Map *m = new Map(loaded->id, loaded->tileset, loaded->tilemap, loaded->width, loaded->height, loaded->layers);
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
                        new MapJSON(1, "map_test", 1, 32, 32, {{{}}}, {"resources/tile00.png"}, {* new Sprite(0, 0, "resources/tile01.png")}, {}));
        myData->tilemap.resize(1);
        for (int l = 0; l < 1; l++) {
            myData->tilemap[l].resize(32);
            for (int h = 0; h < 32; h++) {
                myData->tilemap[l][h].resize(32);
                for (int w = 0; w < 32; w++) {
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
    for (auto spr : sprites) {
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