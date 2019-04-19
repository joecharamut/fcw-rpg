#include <utility>

#include <iostream>
#include <memory>
#include <experimental/filesystem>
#include <algorithm>

#include "Map.h"
#include "Util.h"
#include "Main.h"
#include "Log.h"
#include "Engine.h"
#include "module/Registries.h"

Map::Map(std::string id, std::string defaultRoom, std::vector<std::string> rooms,
         std::map<std::string, Text> textsString, std::map<std::string, std::string> soundEffectsString,
         std::map<std::string, std::string> musicString) {
    this->id = std::move(id);
    this->defaultRoom = defaultRoom;

    for (const auto &roomStr : rooms) {
        Room *room = Registries::roomRegistry.get(roomStr);
        this->rooms[room->id] = room;
    }
    this->current_room = this->rooms[defaultRoom];

    // TODO: Have texts hidden until event trigger?
    for (const auto &text : textsString) {
        this->texts[text.first] = new Text(text.second);
    }

    for (const auto &sfx : soundEffectsString) {
        this->soundEffects[sfx.first] = al_create_sample_instance(Engine::loadSample(sfx.second.c_str()));
    }

    for (const auto &str : musicString) {
        this->music[str.first] = al_create_sample_instance(Engine::loadSample(str.second.c_str()));
    }
}

std::vector<Object *> Map::getObjects() {
    return current_room->objects;
}

Object* Map::getObjectById(std::string id) {
    return current_room->getObjectById(std::move(id));
}

Object* Map::checkCollision(Object *object) {
    return current_room->checkCollision(object);
}

void Map::updateViewport(Object *obj, bool override) {
    current_room->updateViewport(obj, override);
}

void Map::draw() {
    current_room->draw();

    // For the texts, draw them
    // TODO: Make this event controlled. Maybe something similar to music module, but scrolling text and playing a sound
    // TODO: Fix this / replace with text boxes
    // TODO: Why was this breaking everything
    /*for (const auto &textMap : texts) {
        auto text = textMap.second;
        al_draw_text(Engine::fonts[text->font], al_map_rgb(text->r, text->g, text->b), text->x, text->y, 0,
                text->text.c_str());
    }*/
}

void Map::handleEvent(ALLEGRO_EVENT event) {
    if (handlerFunction) {
        handlerFunction(event);
    }
}

void Map::setEventHandlerFunction(void (*handler)(ALLEGRO_EVENT)) {
    this->handlerFunction = handler;
}

void Map::setRoom(std::string roomId) {
    if (!rooms.count(roomId)) {
        return;
    }

    current_room = rooms[roomId];
}