#include "object/Object.h"
#include "module/Registries.h"

Object::Object(std::string id, float x, float y, std::map<std::string, Sprite *> sprites, CollisionType collision) {
    this->id = std::move(id);
    this->x = x;
    this->y = y;
    this->sprites = std::move(sprites);
    this->collision = collision;
}

Object::Object(std::string id, float x, float y, std::map<std::string, std::string> sprites, CollisionType collision) {
    this->id = std::move(id);
    this->x = x;
    this->y = y;
    this->collision = collision;

    for (const auto &s : sprites) {
        this->sprites[s.first] = Registries::spriteRegistry.get(s.second);
    }
}

void Object::draw() {
    al_draw_bitmap(this->sprites[currentSprite]->getNextFrame(), x, y, 0);
}

void Object::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
    updateBoundingBox();
}

void Object::setDisplace(float dX, float dY) {
    this->dX = dX;
    this->dY = dY;
    updateBoundingBox();
}

void Object::updateBoundingBox() {
    if (boundingBox) {
        delete boundingBox;
    }
    boundingBox = new BoundingBox(x-dX, y-dY, x-dX+getWidth(), y-dY+getHeight());
}
