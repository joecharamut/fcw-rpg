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
#include "Event.h"
#include <fstream>
#include <allegro5/allegro_audio.h>

struct Text {
    std::string text;
    float x = 0, y = 0;
    std::string font;
    unsigned char r = 0, g = 0, b = 0;

    Text() = default;;
    Text(std::string text, float x, float y, std::string font, unsigned char r, unsigned char g, unsigned char b) {
        this->text = std::move(text);
        this->x = x;
        this->y = y;
        this->font = std::move(font);
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
};

struct MapJSON {
    int version = 1;
    std::string id;
    std::vector<std::vector<std::vector<int>>> tilemap;
    std::vector<std::string> tileset;
    std::vector<Sprite> sprites;
    std::vector<std::string> events;
    std::vector<Text> texts;
    std::vector<std::string> music;

    MapJSON() = default;;

    MapJSON(int version, std::string id,
            std::vector<std::vector<std::vector<int>>> tilemap, std::vector<std::string> tileset,
            std::vector<Sprite> sprites, std::vector<std::string> events, std::vector<Text> texts,
            std::vector<std::string>music) {
        this->version = version;
        this->id = std::move(id);
        this->tilemap = std::move(tilemap);
        this->tileset = std::move(tileset);
        this->sprites = std::move(sprites);
        this->events = std::move(events);
        this->texts = std::move(texts);
        this->music = std::move(music);
    }

    template <class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(version),
                CEREAL_NVP(id),
                CEREAL_NVP(tilemap),
                CEREAL_NVP(tileset),
                CEREAL_NVP(sprites),
                CEREAL_NVP(events),
                CEREAL_NVP(texts),
                CEREAL_NVP(music)
        );
    }
};

class Map {
public:
    std::string id;
    std::vector<ALLEGRO_BITMAP *> backgrounds = {};
    std::vector<Sprite *> sprites = {};
    std::vector<Text *> texts = {};
    std::vector<ALLEGRO_SAMPLE *> music = {};

    std::vector<Event *> events = {};

    float viewportX = 0;
    float viewportY = 0;

    void (*handlerFunction)(ALLEGRO_EVENT event);

    static Map* loadMap(std::string id);
    static Map* loadMapFile(std::string filename);
    Map(std::string id, std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap,
            std::vector<Sprite> sprites, std::vector<std::string> events, std::vector<Text> texts,
            std::vector<std::string> music);
    static std::vector<std::string> enumerateMaps();

    std::string getFilePath(std::string filename);
    void resolveMap(std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap);
    void draw();

    void addSprite(Sprite *sprite);
    void addText(std::string text, std::string font,
            unsigned char r, unsigned char g, unsigned char b, float x, float y);
    Sprite* getSpriteById(std::string id);
    void handleEvent(ALLEGRO_EVENT event);
    void setEventHandlerFunction(void (*handler)(ALLEGRO_EVENT event));
    Sprite* checkCollision(Sprite *sprite);
    void updateViewport(Sprite *spr, bool override);

};

#endif //FCWRPG_MAP_H
