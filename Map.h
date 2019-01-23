
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

#include "Event.h"
class Event;
class Map;
class Room;

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

struct RoomJSON {
    int version = 1;
    std::string id;
    std::vector<std::vector<std::vector<int>>> tilemap;
    std::vector<std::string> tileset;
    std::vector<Sprite> sprites;
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

struct MapJSON {
    int version = 1;
    std::string id;
    std::string defaultRoom;
    std::vector<std::string> roomFiles;
    std::map<std::string, Text> textsString;
    std::map<std::string, std::string> soundEffectsString;
    std::map<std::string, std::string> musicString;

    MapJSON() = default;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(version),
                CEREAL_NVP(id),
                CEREAL_NVP(defaultRoom),
                CEREAL_NVP(roomFiles),
                cereal::make_nvp("texts", textsString),
                cereal::make_nvp("soundEffects", soundEffectsString),
                cereal::make_nvp("music", musicString)
        );
    }
};

class Room {
public:
    Map *parent;

    std::string id;
    std::vector<ALLEGRO_BITMAP *> backgrounds = {};
    std::vector<Sprite *> sprites = {};
    std::vector<Event *> events = {};
    float viewportX = 0;
    float viewportY = 0;

    Room(std::string id, std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap,
        std::vector<Sprite> sprites, std::vector<std::string> events, Map *parent);

    std::vector<Animation *> resolveTileset(std::vector<std::string> in);
    void resolveMap(std::vector<std::string> tileset, std::vector<std::vector<std::vector<int>>> tilemap);
    void updateViewport(Sprite *spr, bool override);
    Sprite* getSpriteById(std::string id);
    Sprite* checkCollision(Sprite *sprite);
    void draw();
};

class Map {
public:
    bool loaded = false;

    std::string id;
    std::string defaultRoom;
    std::map<std::string, Room *> rooms;
    std::map<std::string, Text *> texts;
    std::map<std::string, ALLEGRO_SAMPLE_INSTANCE *> soundEffects;
    std::map<std::string, ALLEGRO_SAMPLE_INSTANCE *> music;
    Room *current_room;

    void (*handlerFunction)(ALLEGRO_EVENT event);

    static Map* loadMap(std::string id);
    static Map* loadMapFile(std::string filename);
    static std::vector<std::string> enumerateMaps();
    static std::string getFilePath(std::string filename, Map *map);

    void handleEvent(ALLEGRO_EVENT event);
    void setEventHandlerFunction(void (*handler)(ALLEGRO_EVENT event));


    // Pass to current room
    std::vector<Sprite *> getSprites();
    Sprite* getSpriteById(std::string id);
    Sprite* checkCollision(Sprite *sprite);
    void updateViewport(Sprite *spr, bool override);
    void draw();

    Map(std::string id, std::string defaultRoom, std::vector<std::string> roomFiles,
            std::map<std::string, Text> textsString, std::map<std::string, std::string> soundEffectsString,
            std::map<std::string, std::string> musicString);
};

#endif //FCWRPG_MAP_H
