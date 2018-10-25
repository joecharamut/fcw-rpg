#include <memory>


#include "Map.h"
#include "Globals.h"
#include "ActionSprite.h"

Map::Map(const char *id, std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap, int length, int height, int layers) {
    this->id = id;
    //this->tileset = tileset;
    this->tilemap = resolveMap(tileset, tilemap, length, height, layers);
    this->length = length;
    this->height = height;
    this->layers = layers;
    this->sprites = new LinkedSprite();
    this->texts = new LinkedText();
}

Map *Map::loadMap(const char *filename) {
    std::ifstream is(filename, std::ios::binary);
    if (!is.fail()) {
        cereal::JSONInputArchive inputArchive(is);
        std::unique_ptr<MapJSON> loaded{nullptr};
        inputArchive(cereal::make_nvp("mapdata", loaded));
        printf("Loading Map %s", loaded->id.c_str());
        Map *m = new Map(loaded->id.c_str(), loaded->tileset, loaded->tilemap, loaded->width, loaded->height, loaded->layers);
        return m;
    } else {
        printf("Error loading map %s", filename);
    }
    return nullptr;
}

void Map::test() {
    {
        std::ofstream os("test.json", std::ios::binary);
        cereal::JSONOutputArchive archive(os);
        std::unique_ptr<MapJSON> myData = std::make_unique<MapJSON>(new MapJSON(1, "map_test", 1, 32, 32, {{{}}}, {"resources/tile00.png"}));
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
                resolved[l][x][y] = new Tile(tileset[tilemap[l][x][y]].c_str());
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
            for (int y = 0; y < height; y++) {
                tilemap[l][x][y]->draw();
            }
        }
    }
    LinkedText *nextText = texts;
    while(nextText != nullptr) {
        if (nextText->text == nullptr) {
            nextText = nextText->next;
            continue;
        }
        al_draw_text(nextText->font, nextText->color, nextText->x, nextText->y, 0, nextText->text);
        nextText = nextText->next;
    }
    LinkedSprite *next = sprites;
    while(next != nullptr) {
        if (next->sprite == nullptr) {
            next = next->next;
            continue;
        }
        next->sprite->draw();
        next = next->next;
    }
}

bool Map::checkCollision(Sprite *sprite) {
    BoundingBox *box = sprite->boundingBox;
    for (int l = 0; l < layers; l++) {
        for (int x = 0; x < height; x++) {
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
    LinkedSprite *add = new LinkedSprite();
    add->sprite = sprite;

    LinkedSprite *next = sprites;
    while(next->next != nullptr) {
        next = next->next;
    }
    next->next = add;
}

void Map::addText(const char *text, ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y) {
    LinkedText *add = new LinkedText();
    add->text = text;
    add->x = x;
    add->y = y;
    add->font = font;
    add->color = color;
    
    LinkedText *next = texts;
    while(next->next != nullptr) {
        next = next->next;
    }
    next->next = add;
}

Sprite* Map::getSpriteById(const char *id) {
    LinkedSprite *next = sprites;
    while(next != nullptr) {
        if (next->sprite == nullptr) {
            next = next->next;
            continue;
        }
        if (strcmp(next->sprite->id, id) == 0) {
            return next->sprite;
        }
        next = next->next;
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