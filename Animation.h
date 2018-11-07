#include <utility>

#ifndef FCWRPG_ANIMATION_H
#define FCWRPG_ANIMATION_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <vector>
#include <cereal/cereal.hpp>

class Animation {
public:
    std::string type;
    std::vector<std::string> frames;
    std::vector<ALLEGRO_BITMAP*> loadedFrames;
    int speed = 1;

    int speedCount = 0;
    int currentFrame = 0;

    Animation() = default;;
    Animation(std::string type, std::vector<std::string> frames, int speed);
    Animation(std::string image) : Animation("default", {std::move(image)}, 1) {};

    void loadFrames();
    ALLEGRO_BITMAP* nextFrame();

    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(type), CEREAL_NVP(frames), CEREAL_NVP(speed));
    }

    template <class Archive>
    static void load_and_construct(Archive &archive, cereal::construct<Animation> &construct) {
        std::string type;
        std::vector<std::string> frames;
        int speed;

        archive(CEREAL_NVP(type), CEREAL_NVP(frames), CEREAL_NVP(speed));
        construct(type, frames, speed);
    }
};


#endif //FCWRPG_ANIMATION_H
