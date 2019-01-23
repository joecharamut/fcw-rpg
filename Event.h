
#ifndef FCWRPG_EVENT_H
#define FCWRPG_EVENT_H

#include <utility>
#include <vector>
#include <string>
#include <map>
#include <allegro5/keycodes.h>
#include "Keyboard.h"

#include "Map.h"
class Map;

class Event {
public:
    int timer = 0;
    int delay = 0;

    static void test();
};



#endif //FCWRPG_EVENT_H
