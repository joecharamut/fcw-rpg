#ifndef FCWRPG_ROOM_H
#define FCWRPG_ROOM_H

#include <string>
#include <vector>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/map.hpp"

#include "Sprite.h"
#include "Map.h"
#include "Event.h"

class Map;
class Event;

struct RoomJSON {
    int version = 1;
    std::string id;
    std::vector<std::vector<std::vector<int>>> tilemap;
    std::vector<std::string> tileset;
    std::vector<std::string> sprites;
    std::vector<std::string> events;

    RoomJSON() = default;

    template <class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(version),
                CEREAL_NVP(id),
                CEREAL_NVP(tilemap),
                CEREAL_NVP(tileset),
                CEREAL_NVP(sprites),
                CEREAL_NVP(events)
        );
    }
};

class Room {
public:
    std::string id;
    std::vector<ALLEGRO_BITMAP *> backgrounds = {};
    std::vector<Sprite *> sprites = {};
    std::vector<Event *> events = {};
    float viewportX = 0;
    float viewportY = 0;

    Room(std::string id, std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap,
        std::vector<std::string> sprites, std::vector<std::string> events);

    std::vector<Animation *> resolveTileset(std::vector<std::string> in);
    void resolveMap(std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap);
    void updateViewport(Sprite *spr, bool override);
    Sprite* getSpriteById(std::string id);
    Sprite* checkCollision(Sprite *sprite);
    void draw();
};


#endif //FCWRPG_ROOM_H
