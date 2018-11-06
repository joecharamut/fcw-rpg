#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <vector>
#include <cereal/cereal.hpp>
#include "BoundingBox.h"

#ifndef FCWRPG_SPRITE_H
#define FCWRPG_SPRITE_H

enum COLLISION_TYPE {
    NONE,
    TILE
};

class Sprite {
public:
    COLLISION_TYPE collision;

    float x;
    float y;
    std::vector<std::string> frameStr;
    std::vector<ALLEGRO_BITMAP *> frames;
    int width;
    int height;
    std::string id;
    //std::string imageName;

    int currentFrame = 0;
    int speed = 0;
    int speedCount = 0;

    Sprite() = default;;
    explicit Sprite(Sprite *spr) : Sprite(spr->x, spr->y, spr->id, spr->frameStr, spr->speed) {};
    Sprite(float x, float y, std::string id, std::string image);
    Sprite(float x, float y, std::string image) : Sprite(x, y, "", image) {};
    Sprite(float x , float y, std::string id, std::vector<std::string> frames, int speed);
    virtual void draw();
    virtual void setX(float newX);
    virtual void setY(float newY);
    BoundingBox *boundingBox;
    void updateBoundingBox();
    void addFrame(std::string image);

    template <class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(id),
                CEREAL_NVP(x),
                CEREAL_NVP(y),
                CEREAL_NVP(frameStr),
                CEREAL_NVP(speed)
        );
    }

    template <class Archive>
    static void load_and_construct(Archive &archive, cereal::construct<Sprite> &construct) {
        float x;
        float y;
        std::vector<std::string> frameStr;
        std::string id;
        int speed = 0;
        archive(
                CEREAL_NVP(id),
                CEREAL_NVP(x),
                CEREAL_NVP(y),
                CEREAL_NVP(frameStr),
                CEREAL_NVP(speed)
        );
        construct(
                id,
                x,
                y,
                frameStr,
                speed
        );
    }
};

#endif //FCWRPG_SPRITE_H
