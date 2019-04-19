#include "Room.h"

#include "Engine.h"
#include "module/Registries.h"

// Constructor
Room::Room(std::string id, std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap,
           std::vector<ObjectJSON> objects) {
    // Copy in id
    this->id = std::move(id);

    // Load the tilemap as actual images into memory
    resolveMap(std::move(tileset), std::move(tilemap));

    // Load in the sprites
    for (auto obj : objects) {
        this->objects.push_back(obj.toObject());
    }
}

// Function to resolve the tileset to a list of Animations with the frames loaded in memory
std::vector<Sprite *> Room::resolveTileset(std::vector<std::string> in) {
    // Create out array
    std::vector<Sprite *> out = {};
    // Reserve space
    out.reserve(in.size());
    // For each tile, create an Animation and store it
    for (const auto &tile : in) {
        out.push_back(new Sprite(tile));
    }
    return out;
}

// Function to resolve the tileset and tilemap into images
void Room::resolveMap(std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap) {
    // Get layers, width, and height
    auto layers = (int) tilemap.size();
    auto width = (int) tilemap[0].size();
    auto height  = (int) tilemap[0][0].size();

    // Get the tileset as images
    std::vector<Sprite *> spriteMap = resolveTileset(std::move(tileset));
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
                    al_draw_bitmap(spriteMap[tilemap[l][x][y]]->getNextFrame(), x*32, y*32, 0);
                }
            }
        }
    }
    // Reset the target to the display
    al_set_target_backbuffer(al_get_current_display());
}

// Function to update the viewport based on a sprite (Presumably the player)
void Room::updateViewport(Object *obj, bool override) {
    bool changed = false;

    float cX = Engine::SCREEN_W/2.0f;
    float cY = Engine::SCREEN_H/2.0f;
    float csX = (obj->x + (obj->getWidth()/2.0f));
    float csY = (obj->y + (obj->getHeight()/2.0f));
    if (csX != cX || csY != cY) {
        float dX = cX - csX;
        float dY = cY - csY;
        if (override) {
            viewportX -= dX;
            viewportY -= dY;
            obj->setPosition(obj->x+dX, obj->y+dY);
            changed = true;
        } else {
            float newX = viewportX - dX;
            float newY = viewportY - dY;
            if (newX >= 0 && newX <= al_get_bitmap_width(backgrounds[0])-Engine::SCREEN_W) {
                viewportX -= dX;
                obj->setPosition(obj->x+dX, obj->y);
                changed = true;
            }
            if (newY >= 0 && newY <= al_get_bitmap_height(backgrounds[0])-Engine::SCREEN_H) {
                viewportY -= dY;
                obj->setPosition(obj->x, obj->y+dY);
                changed = true;
            }
        }
        if (changed) {
            for (Object *object : objects) {
                if (object->id != obj->id) {
                    object->setDisplace(viewportX, viewportY);
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
    for (auto *obj : objects) {
        obj->draw();
    }

    // Check and execute events
    /*for (auto *event : events) {

    }*/
}

Object *Room::getObjectById(std::string id) {
    for (auto *obj : objects) {
        if (obj->id == id) {
            return obj;
        }
    }
    return nullptr;
}

Object* Room::checkCollision(Object *object) {
    BoundingBox *box = object->boundingBox;
    for (auto *obj : objects) {
        if (obj->collision == COLLISION_NONE) {
            continue;
        }
        // TODO: Fix this
        //obj->updateBoundingBox();
        BoundingBox *check = obj->boundingBox;
        if (BoundingBox::intersect(check, box)) {
            return obj;
        }
    }
    return nullptr;
}
