#ifndef FCWRPG_EVENT_H
#define FCWRPG_EVENT_H

#include <utility>
#include <vector>
#include <string>
#include <map>
#include <allegro5/keycodes.h>
#include "Keyboard.h"
#include <duktape-cpp/DuktapeCpp.h>

#include "Map.h"
class Map;

class Event {
public:
    std::string eventCode;

    Event(std::string encodedEvent);
    void execute();

    static void eval(std::string str);

private:
    static duk::Context initContext();
};



#endif //FCWRPG_EVENT_H
