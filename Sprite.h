#ifndef FCWRPG_SPRITE_H
#define FCWRPG_SPRITE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <vector>
#include <cereal/cereal.hpp>
#include "BoundingBox.h"
#include "Animation.h"
#include "Vec2D.h"

enum COLLISION_TYPE {
    NONE,
    TILE
};

class Sprite {
public:
    COLLISION_TYPE collision = NONE;

    float x = 0;
    float y = 0;

    float lastX = (2<<16);
    float lastY = (2<<16);

    float lastDelta = (2<<16);

    Vec2D velocity;
    std::vector<Animation> frames;
    int width;
    int height;
    std::string id;
    int animation = 0;
    BoundingBox *boundingBox;
    float dX = 0;
    float dY = 0;

    Sprite() = default;
    Sprite(float x, float y, std::string id, std::vector<Animation> frames, COLLISION_TYPE collision);
    explicit Sprite(Sprite *spr) : Sprite(spr->x, spr->y, spr->id, spr->frames, spr->collision) {};//{ this->collision = spr->collision; };

    virtual void draw();
    virtual void setX(float newX);
    virtual void setY(float newY);
    virtual float getX();
    virtual float getY();
    void update(float delta);
    void setVelocity(Vec2D newVelocity);
    static Vec2D TCV(Vec2D thisPos, Vec2D lastPos, float thisDelta, float lastDelta, Vec2D acceleration);
    void setDisplace(float dX, float dY);
    void updateBoundingBox();

    template <class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(id),
                CEREAL_NVP(x),
                CEREAL_NVP(y),
                CEREAL_NVP(collision),
                CEREAL_NVP(frames)
        );
    }
};

#endif //FCWRPG_SPRITE_H
