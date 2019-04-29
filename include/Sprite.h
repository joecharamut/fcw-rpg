#include <utility>

#ifndef FCWRPG_SPRITE_H
#define FCWRPG_SPRITE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <vector>
#include <string>
#include <BoundingBox.h>

class Sprite {
public:
    std::string id;
    std::vector<ALLEGRO_BITMAP *> frames;

    int speed = 1;
    int currentFrame = 0;
    int counter = 0;

    Sprite(std::string id, std::vector<ALLEGRO_BITMAP *> frames, int speed);
    Sprite(std::string id, std::vector<std::string> frameStrings, int speed);
    explicit Sprite(std::string frame) : Sprite("null", {std::move(frame)}, 1) {};
    Sprite(const Sprite &spr) : Sprite(spr.id, spr.frames, spr.speed) {};

    int getWidth();
    int getHeight();

    void unload();

    ALLEGRO_BITMAP *getNextFrame();
};

#endif //FCWRPG_SPRITE_H
