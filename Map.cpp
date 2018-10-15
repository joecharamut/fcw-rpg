

#include "Map.h"
#include "Globals.h"
#include "ActionSprite.h"

Map::Map(int id, const char *name, Tile **tileset, int **tilemap, int length, int height) {
    this->id = id;
    this->name = name;
    this->tileset = tileset;
    this->tilemap = tilemap;
    this->length = length;
    this->height = height;
    this->sprites = new LinkedSprite();
    sprites->sprite = nullptr;
    sprites->next = nullptr;
}

void Map::draw() {
    for (int x = 0; x < height; x++) {
        for (int y = 0; y < length; y++) {
            Tile *tile = tileset[tilemap[x][y]];
            tile->draw((x*tile->width), (y*tile->height));
        }
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
    if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        done = true;
    } else if(event.type == ALLEGRO_EVENT_KEY_UP) {
        switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
        }
    } else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        LinkedSprite *next = sprites;
        while(next != nullptr) {
            if (next->sprite == nullptr) {
                next = next->next;
                continue;
            }
            ActionSprite *actionSprite = dynamic_cast<ActionSprite *>(next->sprite);
            if (actionSprite) {
                if (actionSprite->boundingBox->check(event.mouse.x, event.mouse.y)) {
                    actionSprite->clickAction(actionSprite);
                }
            }
            next = next->next;
        }
    } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
        LinkedSprite *next = sprites;
        while(next != nullptr) {
            if (next->sprite == nullptr) {
                next = next->next;
                continue;
            }
            ActionSprite *actionSprite = dynamic_cast<ActionSprite *>(next->sprite);
            if (actionSprite) {
                if (actionSprite->boundingBox->check(event.mouse.x, event.mouse.y)) {
                    actionSprite->hoverAction(actionSprite);
                }
            }
            next = next->next;
        }
    }
}