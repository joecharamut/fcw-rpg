#include <utility>


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

struct Text {
    std::string text;
    float x = 0, y = 0;
    std::string font;
    unsigned char r = 0, g = 0, b = 0;

    Text() {};
    Text(std::string text, float x, float y, std::string font, unsigned char r, unsigned char g, unsigned char b) {
        this->text = text;
        this->x = x;
        this->y = y;
        this->font = font;
        this->r = r;
        this->g = g;
        this->b = b;
    }

    template <class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(text),
                CEREAL_NVP(x),
                CEREAL_NVP(y),
                CEREAL_NVP(font),
                CEREAL_NVP(r),
                CEREAL_NVP(g),
                CEREAL_NVP(b)
        );
    }

    template <class Archive>
    static void load_and_construct(Archive &archive, cereal::construct<Text> &construct) {
        std::string text;
        float x = 0, y = 0;
        std::string font;
        int r = 0, g = 0, b = 0;
        archive(
                CEREAL_NVP(text),
                CEREAL_NVP(x),
                CEREAL_NVP(y),
                CEREAL_NVP(font),
                CEREAL_NVP(r),
                CEREAL_NVP(g),
                CEREAL_NVP(b)
        );
        construct(
                text,
                x,
                y,
                font,
                r,
                g,
                b
        );
    }
};

struct MapJSON {
    int version;
    std::string id;
    int layers, height, width;
    std::vector<std::vector<std::vector<int>>> tilemap;
    std::vector<std::string> tileset;
    std::vector<Sprite> sprites;
    std::vector<Text> texts;

    MapJSON() = default;;
    MapJSON(int version, std::string id,
            int layers, int height, int width,
            std::vector<std::vector<std::vector<int>>> tilemap, std::vector<std::string> tileset,
            std::vector<Sprite> sprites, std::vector<Text> texts) {
        this->version = version;
        this->id = std::move(id);
        this->layers = layers;
        this->height = height;
        this->width = width;
        this->tilemap = std::move(tilemap);
        this->tileset = std::move(tileset);
        this->sprites = sprites;
        this->texts = texts;
    }

    explicit MapJSON(MapJSON *in) {
        this->version = in->version;
        this->id = in->id;
        this->layers = in->layers;
        this->height = in->height;
        this->width = in->width;
        this->tilemap = in->tilemap;
        this->tileset = in->tileset;
        this->sprites = in->sprites;
        this->texts = in->texts;
    }

    template <class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(version),
                CEREAL_NVP(id),
                CEREAL_NVP(layers),
                CEREAL_NVP(height),
                CEREAL_NVP(width),
                CEREAL_NVP(tilemap),
                CEREAL_NVP(tileset),
                CEREAL_NVP(sprites),
                CEREAL_NVP(texts)
        );
    }

    template <class Archive>
    static void load_and_construct(Archive &archive, cereal::construct<MapJSON> &construct) {
        int version = 0;
        std::string id;
        int layers = 0, height = 0, width = 0;
        std::vector<std::vector<std::vector<int>>> tilemap;
        std::vector<std::string> tileset;
        std::vector<Sprite> sprites;
        std::vector<Text> texts;

        archive(
                CEREAL_NVP(version),
                CEREAL_NVP(id),
                CEREAL_NVP(layers),
                CEREAL_NVP(height),
                CEREAL_NVP(width),
                CEREAL_NVP(tilemap),
                CEREAL_NVP(tileset),
                CEREAL_NVP(sprites),
                CEREAL_NVP(texts)
        );
        construct(
                version,
                id,
                layers,
                height,
                width,
                tilemap,
                tileset,
                sprites,
                texts
        );
    }
};

class Map {
public:
    std::string id;
    std::vector<std::vector<std::vector<Tile *>>> tilemap = {};
    int length;
    int width;
    int layers;
    std::vector<Sprite *> sprites = {};
    std::vector<Text *> texts = {};
    void (*handlerFunction)(ALLEGRO_EVENT event);

    static Map* loadMap(std::string mapname);
    static Map* loadMapFile(std::string filename);
    Map(std::string id, std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap,
            int length, int width, int layers, std::vector<Sprite> sprites, std::vector<Text> texts);
    static void test();

    static std::vector<std::string> enumerateMaps();
    std::vector<std::vector<std::vector<Tile *>>> resolveMap(std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap, int length, int height, int layers);
    void draw();
    void addSprite(Sprite *sprite);
    void addText(std::string text, std::string font, unsigned char r, unsigned char g, unsigned char b, float x, float y);
    Sprite* getSpriteById(std::string id);
    void handleEvent(ALLEGRO_EVENT event);
    void setEventHandlerFunction(void (*handler)(ALLEGRO_EVENT event));
    Sprite* checkCollision(Sprite *sprite);


};

#endif //FCWRPG_MAP_H
