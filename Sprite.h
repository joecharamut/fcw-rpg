#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <vector>
#include <cereal/cereal.hpp>
#include "BoundingBox.h"
#include "Animation.h"

#ifndef FCWRPG_SPRITE_H
#define FCWRPG_SPRITE_H

enum COLLISION_TYPE {
    NONE,
    TILE
};

class Sprite {
public:
    COLLISION_TYPE collision = NONE;

    float x;
    float y;
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
    //Sprite(float x, float y, std::string id, Animation image) : Sprite(x, y, id, (std::vector<Animation>) {image}) {};
    //Sprite(float x, float y, std::vector<Animation> frames) : Sprite(x, y, "", frames) {};
    //Sprite(float x, float y, Animation image) : Sprite(x, y, "", image) {};
    explicit Sprite(Sprite *spr) : Sprite(spr->x, spr->y, spr->id, spr->frames, spr->collision) {};//{ this->collision = spr->collision; };

    virtual void draw();
    virtual void setX(float newX);
    virtual void setY(float newY);
    virtual float getX();
    virtual float getY();
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
