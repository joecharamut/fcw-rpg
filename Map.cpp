

#include "Map.h"
#include "Globals.h"
#include "ActionSprite.h"

Map::Map(int id, const char *name, Tile **tileset, int ***tilemap, int length, int height, int layers) {
    this->id = id;
    this->name = name;
    this->tileset = tileset;
    this->tilemap = tilemap;
    this->length = length;
    this->height = height;
    this->layers = layers;
    this->sprites = new LinkedSprite();
    this->texts = new LinkedText();
}

void Map::draw() {
    for (int l = 0; l < layers; l++) {
        for (int x = 0; x < height; x++) {
            for (int y = 0; y < length; y++) {
                Tile *tile = tileset[tilemap[l][x][y]];
                tile->draw((x * tile->width), (y * tile->height));
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
