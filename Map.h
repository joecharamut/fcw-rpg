

#ifndef FCWRPG_MAP_H
#define FCWRPG_MAP_H


#include <allegro5/allegro_font.h>
#include "Tile.h"

#include "cereal/cereal.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/archives/json.hpp"
#include <fstream>

struct LinkedText {
    const char *text;
    float x, y;
    ALLEGRO_FONT *font;
    ALLEGRO_COLOR color;
    LinkedText *next;
};

struct MapJSON {
    int version;
    std::unique_ptr<std::string> id;

    int layers, height, width;
    // TODO: fix this
    int tilemap;
    //Tile *tileset;

    template <class Archive>
    void save(Archive &archive) const {
        archive(version, id, layers, height, width);
    }

    template <class Archive>
    void load(Archive &archive) {
        archive(version, id, layers, height, width);
    }
};

class Map {
public:
    int id;
    const char *name;
    //Tile **tileset;
    //int ***tilemap;
    Tile ****tilemap;
    int length;
    int height;
    int layers;
    LinkedSprite *sprites;
    LinkedText *texts;
    void (*handlerFunction)(ALLEGRO_EVENT event);

    static Map* loadMap(const char *filename);
    Map(int id, const char *name, Tile **tileset, int ***tilemap, int length, int height, int layers);

    Tile ****resolveMap(Tile **tileset, int ***tilemap, int length, int height, int layers);
    void draw();
    void addSprite(Sprite *sprite);
    void addText(const char *text, ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y);
    Sprite* getSpriteById(const char *id);
    void handleEvent(ALLEGRO_EVENT event);
    void setEventHandlerFunction(void (*handler)(ALLEGRO_EVENT event));
    bool checkCollision(Sprite *sprite);
};

#endif //FCWRPG_MAP_H
