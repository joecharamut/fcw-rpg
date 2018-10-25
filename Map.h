

#ifndef FCWRPG_MAP_H
#define FCWRPG_MAP_H


#include <allegro5/allegro_font.h>
#include "Tile.h"

#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include <fstream>

struct LinkedText {
    const char *text;
    float x, y;
    ALLEGRO_FONT *font;
    ALLEGRO_COLOR color;
    LinkedText *next;
};

struct MapJSON {

    MapJSON(int version, std::string id,
            int layers, int height, int width,
            std::vector<std::vector<std::vector<int>>> tilemap, std::vector<std::string> tileset) {
        this->version = version;
        this->id = id;
        this->layers = layers;
        this->height = height;
        this->width = width;
        this->tilemap = tilemap;
        this->tileset = tileset;
    }

    explicit MapJSON(MapJSON *in) {
        this->version = in->version;
        this->id = in->id;
        this->layers = in->layers;
        this->height = in->height;
        this->width = in->width;
        this->tilemap = in->tilemap;
        this->tileset = in->tileset;
    }

    int version;
    std::string id;
    int layers, height, width;
    std::vector<std::vector<std::vector<int>>> tilemap;
    std::vector<std::string> tileset;

    template <class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(version),
                CEREAL_NVP(id),
                CEREAL_NVP(layers),
                CEREAL_NVP(height),
                CEREAL_NVP(width),
                CEREAL_NVP(tilemap),
                CEREAL_NVP(tileset)
        );
    }

    template <class Archive>
    static void load_and_construct(Archive &archive, cereal::construct<MapJSON> &construct) {
        int version;
        std::string id;
        int layers, height, width;
        std::vector<std::vector<std::vector<int>>> tilemap;
        std::vector<std::string> tileset;

        archive(
                CEREAL_NVP(version),
                CEREAL_NVP(id),
                CEREAL_NVP(layers),
                CEREAL_NVP(height),
                CEREAL_NVP(width),
                CEREAL_NVP(tilemap),
                CEREAL_NVP(tileset)
        );
        construct(
                version,
                id,
                layers,
                height,
                width,
                tilemap,
                tileset
        );
    }
};

class Map {
public:
    const char *id;
    std::vector<std::vector<std::vector<Tile *>>> tilemap;
    int length;
    int height;
    int layers;
    LinkedSprite *sprites;
    LinkedText *texts;
    void (*handlerFunction)(ALLEGRO_EVENT event);

    static Map* loadMap(const char *filename);
    Map(const char *id, std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap, int length, int height, int layers);
    static void test();

    std::vector<std::vector<std::vector<Tile *>>> resolveMap(std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap, int length, int height, int layers);
    void draw();
    void addSprite(Sprite *sprite);
    void addText(const char *text, ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y);
    Sprite* getSpriteById(const char *id);
    void handleEvent(ALLEGRO_EVENT event);
    void setEventHandlerFunction(void (*handler)(ALLEGRO_EVENT event));
    bool checkCollision(Sprite *sprite);
};

#endif //FCWRPG_MAP_H
