#include <memory>


#include "Map.h"
#include "Globals.h"
#include "ActionSprite.h"

Map::Map(int id, const char *name, Tile **tileset, int ***tilemap, int length, int height, int layers) {
    this->id = id;
    this->name = name;
    //this->tileset = tileset;
    this->tilemap = resolveMap(tileset, tilemap, length, height, layers);
    this->length = length;
    this->height = height;
    this->layers = layers;
    this->sprites = new LinkedSprite();
    this->texts = new LinkedText();
}

Map* Map::loadMap(const char *filename) {
    std::ofstream os("test.json", std::ios::binary);
    cereal::JSONOutputArchive archive(os);
    MapJSON myData;
    myData.id = std::make_unique<std::string>("map_test");
    myData.layers = 1;
    myData.height = 1;
    myData.width = 1;
    myData.version = 1;
    myData.tilemap = (int ***) new int[1][1][1];
    //myData.tilemap[1][1][1] = 0;
    archive(myData);
}

Tile ****Map::resolveMap(Tile **tileset, int ***tilemap, int length, int height, int layers) {
    Tile ****resolved;// = (Tile ****)malloc(layers * length * height * sizeof(Tile *));
    resolved = new Tile***[layers];
    for (int l = 0; l < layers; l++) {
        resolved[l] = new Tile**[length];
        for (int x = 0; x < length; x++) {
            resolved[l][x] = new Tile*[height];
            for (int y = 0; y < height; y++) {
                resolved[l][x][y] = nullptr;
            }
        }
    }
    for (int l = 0; l < layers; l++) {
        for (int x = 0; x < length; x++) {
            for (int y = 0; y < height; y++) {
                resolved[l][x][y] = new Tile(tileset[tilemap[l][x][y]]->frames[0], tileset[tilemap[l][x][y]]->collision);
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