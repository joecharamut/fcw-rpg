#include <utility>

#ifndef FCWRPG_SPRITE_H
#define FCWRPG_SPRITE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <vector>
#include <cereal/cereal.hpp>
#include "BoundingBox.h"

enum COLLISION_TYPE {
    COLLISION_NONE_OLD
};

struct SpriteJSON {
    std::string id;
    std::vector<std::string> frames;
    int speed = 1;

    SpriteJSON() = default;

    template <class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(id),
                CEREAL_NVP(frames),
                CEREAL_NVP(speed)
        );
    }
};

class Sprite {
public:
    std::string id;
    std::vector<ALLEGRO_BITMAP *> frames;
    int width;
    int height;

    int speed = 1;
    int currentFrame = 0;
    int counter = 0;

    Sprite(std::string id, std::vector<ALLEGRO_BITMAP *> frames, int speed);
    Sprite(std::string id, std::vector<std::string> frameStrings, int speed);
    explicit Sprite(std::string frame) : Sprite("null", {std::move(frame)}, 1) {};
    Sprite(const Sprite &spr) : Sprite(spr.id, spr.frames, spr.speed) {};

    ALLEGRO_BITMAP *getNextFrame();
};

#endif //FCWRPG_SPRITE_H
