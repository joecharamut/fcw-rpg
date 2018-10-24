

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
