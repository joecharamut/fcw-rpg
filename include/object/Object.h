#ifndef FCWRPG_OBJECT_H
#define FCWRPG_OBJECT_H

#include <map>
#include <Sprite.h>

enum CollisionType {
    COLLISION_NONE,
    COLLISION_TILE
};

class Object {
public:
    std::string id;
    float x = 0;
    float y = 0;
    float dX = 0;
    float dY = 0;

    BoundingBox *boundingBox = nullptr;
    CollisionType collision;

    Object(std::string id, float x, float y, std::map<std::string, Sprite *> sprites, CollisionType collision);
    Object(std::string id, float x, float y, std::map<std::string, std::string> sprites, CollisionType collision);
    Object(const Object &obj) : Object(obj.id, obj.x, obj.y, obj.sprites, obj.collision) {};

    ~Object();

    void draw();
    void setPosition(float x, float y);
    void setDisplace(float dX, float dY);

    float getX() { return x+dX; };
    float getY() { return y+dY; };
    float getWidth() {return sprites[currentSprite]->width; };
    float getHeight() { return sprites[currentSprite]->height; };

private:
    std::map<std::string, Sprite *> sprites;
    std::string currentSprite = "default";

    void updateBoundingBox();
};

#endif //FCWRPG_OBJECT_H
