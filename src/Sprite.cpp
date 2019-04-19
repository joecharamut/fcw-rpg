#include <utility>

#include "Sprite.h"
#include "Log.h"
#include "Engine.h"

Sprite::Sprite(std::string id, std::vector<ALLEGRO_BITMAP *> frames, int speed) {
    this->id = std::move(id);
    this->frames = std::move(frames);
    this->speed = speed;
}

Sprite::Sprite(std::string id, std::vector<std::string> frameStrings, int speed) {
    this->id = std::move(id);
    for (const std::string &frame : frameStrings) {
        this->frames.push_back(Engine::loadImage(frame.c_str()));
    }
    this->speed = speed;
}

void Sprite::unload() {
    for (ALLEGRO_BITMAP *frame : frames) {
        al_destroy_bitmap(frame);
    }
}

ALLEGRO_BITMAP *Sprite::getNextFrame() {
    if (frames.empty()) Log::errorf("Sprite %s has no frames!", id.c_str());

    counter++;
    if (counter >= speed) {
        counter = 0;
        ++currentFrame %= frames.size();
    }
    // Return the frame at the current index
    return frames[currentFrame];
}

