#ifndef FCWRPG_MAP_H
#define FCWRPG_MAP_H

#include <utility>
#include <fstream>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/map.hpp"

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>

#include <object/Object.h>

struct MapJSON {
    std::string id;
    std::vector<std::vector<std::vector<int>>> tilemap;
    std::vector<std::string> tileset;
    std::vector<std::string> objects;

    MapJSON() = default;

    template <class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(id),
                CEREAL_NVP(tilemap),
                CEREAL_NVP(tileset),
                CEREAL_NVP(objects)
        );
    }
};

class Map {
public:
    std::string id;
    void (*handlerFunction)(ALLEGRO_EVENT event) = nullptr;

    Map(std::string id, std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap,
        std::vector<std::string> objects);
    Map() = default;

    void handleEvent(ALLEGRO_EVENT event);
    void setEventHandlerFunction(void (*handler)(ALLEGRO_EVENT event));

    std::vector<Object *> getObjects();
    Object* getObjectById(std::string id);
    Object* checkCollision(Object *object);

    void updateViewport(Object *obj, bool override);
    void draw();

private:
    std::vector<ALLEGRO_BITMAP *> backgrounds = {};
    std::vector<Object *> objects = {};
    float viewportX = 0;
    float viewportY = 0;

    std::vector<Sprite *> resolveTileset(std::vector<std::string> in);
    void resolveTilemap(std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap);
};

#endif //FCWRPG_MAP_H
