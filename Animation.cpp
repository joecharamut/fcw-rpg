#include <utility>

#include <utility>

#include <utility>

#include "Animation.h"
#include "Util.h"

// Constructor
Animation::Animation(std::string type, std::vector<std::string> frames, int speed) {
    // Copy in type, animation frames, and speed
    this->type = std::move(type);
    this->frames = std::move(frames);
    this->speed = speed;
    // Load the frames into memory
    loadFrames();
}

// Function to load the frames into memory
void Animation::loadFrames() {
    // For each frame
    for (const auto &frame : frames) {
        // Load it into the loadedFrames vector
        loadedFrames.push_back(al_load_bitmap(frame.c_str()));
    }
}

// Function to get the next frame of animation
ALLEGRO_BITMAP* Animation::nextFrame() {
    // If frames are not loaded, load the frames
    if (loadedFrames.empty()) loadFrames();
    // Increment the counter and check if it is more than the speed value
    speedCount++;
    if (speedCount >= speed) {
        // If it is, reset the counter and update the current frame index
        speedCount = 0;
        ++currentFrame %= loadedFrames.size();
    }
    // Return the frame at the current index
    return loadedFrames[currentFrame];
}
