#include <utility>

#include "Animation.h"

Animation::Animation(std::string type, std::vector<std::string> frames, int speed) {
    this->type = std::move(type);
    this->frames = std::move(frames);
    this->speed = speed;
    loadFrames();
}

void Animation::loadFrames() {
    for (const auto &frame : frames) {
        loadedFrames.push_back(al_load_bitmap(frame.c_str()));
    }
}

ALLEGRO_BITMAP* Animation::nextFrame() {
    if (loadedFrames.empty()) loadFrames();
    ++speedCount;
    if (speedCount >= speed) {
        speedCount = 0;
        ++currentFrame %= loadedFrames.size();
    }
    return loadedFrames[currentFrame];
}
