#include "Room.h"

#include "Engine.h"

// Constructor
Room::Room(std::string id, std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap,
           std::vector<std::string> sprites, std::vector<std::string> events) {
    // Copy in id
    this->id = std::move(id);

    // Load the tilemap as actual images into memory
    resolveMap(std::move(tileset), std::move(tilemap));

    // Load in the sprites
    for (auto sprite : sprites) {
        Sprite *spr = (Sprite *) Engine::resourceFileRegistry.get(sprite)->data;
        this->sprites.push_back(spr);
    }

    // Load in events
    for (const auto &eventStr : events) {
        this->events.push_back(new Event(eventStr));
    }
}

// Function to resolve the tileset to a list of Animations with the frames loaded in memory
std::vector<Animation *> Room::resolveTileset(std::vector<std::string> in) {
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
void Room::resolveMap(std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap) {
    // Get layers, width, and height
    int layers = (int) tilemap.size();
    int width = (int) tilemap[0].size();
    int height  = (int) tilemap[0][0].size();

    // Get the tileset as images
    std::vector<Animation *> animationMap = resolveTileset(std::move(tileset));
    // For each layer
    for (int l = 0; l < layers; l++) {
        // Create a new blank bitmap
        backgrounds.push_back(al_create_bitmap(width*32, height*32));
        // Set it as the target
        al_set_target_bitmap(backgrounds[l]);
        // Clear it to transparent
        al_clear_to_color(al_map_rgba(0x00, 0x00, 0x00, 0x00));
        // For every tile in the tilemap
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                // If it's not transparent
                if (tilemap[l][x][y] != -1) {
                    // Draw it to the bitmap
                    al_draw_bitmap(animationMap[tilemap[l][x][y]]->nextFrame(), x*32, y*32, 0);
                }
            }
        }
    }
    // Reset the target to the display
    al_set_target_backbuffer(al_get_current_display());
}

// Function to update the viewport based on a sprite (Presumably the player)
void Room::updateViewport(Sprite *spr, bool override) {
    bool changed = false;

    float cX = Engine::SCREEN_W/2.0f;
    float cY = Engine::SCREEN_H/2.0f;
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
            if (newX >= 0 && newX <= al_get_bitmap_width(backgrounds[0])-Engine::SCREEN_W) {
                viewportX -= dX;
                spr->setX(spr->x+dX);
                changed = true;
            }
            if (newY >= 0 && newY <= al_get_bitmap_height(backgrounds[0])-Engine::SCREEN_H) {
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

// Function to draw a map to the screen
void Room::draw() {
    // For every background image
    for (auto bg : backgrounds) {
        // Draw the viewable portion of it to the screen
        al_draw_bitmap_region(bg, viewportX, viewportY, 512, 512, 0, 0, 0);
    }

    // For each sprite, draw it
    for (auto *spr : sprites) {
        spr->draw();
    }

    // Check and execute events
    for (auto *event : events) {

    }
}

Sprite *Room::getSpriteById(std::string id) {
    for (auto *spr : sprites) {
        if (spr->id == id) {
            return spr;
        }
    }
    return nullptr;
}

Sprite* Room::checkCollision(Sprite *sprite) {
    BoundingBox *box = sprite->boundingBox;
    for (auto *spr : sprites) {
        if (spr->collision == NONE) {
            continue;
        }
        spr->updateBoundingBox();
        BoundingBox *check = spr->boundingBox;
        if (BoundingBox::intersect(check, box)) {
            return spr;
        }
    }
    return nullptr;
}
