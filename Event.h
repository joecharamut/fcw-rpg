
#ifndef FCWRPG_EVENT_H
#define FCWRPG_EVENT_H

#include <utility>
#include <vector>
#include <string>
#include <map>
#include <allegro5/keycodes.h>

#include "Map.h"
class Map;

enum ConditionType {
    NOT_EQUAL, EQUALS, GREATER, LESS, GREATER_OR_EQUAL, LESS_OR_EQUAL
};

enum OperandType {
    TYPE_CONSTANT, TYPE_PROPERTY, TYPE_EVENT, TYPE_STRING, TYPE_KEY
};

enum ActionType {
    ACTION_SET, ACTION_PLAY
};

static std::map<std::string, int> stringToKeyCode = {
        {"KEY_A", ALLEGRO_KEY_A},
        {"KEY_B", ALLEGRO_KEY_B},
        {"KEY_C", ALLEGRO_KEY_C},
        {"KEY_D", ALLEGRO_KEY_D},
        {"KEY_E", ALLEGRO_KEY_E},
        {"KEY_F", ALLEGRO_KEY_F},
        {"KEY_G", ALLEGRO_KEY_G},
        {"KEY_H", ALLEGRO_KEY_H},
        {"KEY_I", ALLEGRO_KEY_I},
        {"KEY_J", ALLEGRO_KEY_J},
        {"KEY_K", ALLEGRO_KEY_K},
        {"KEY_L", ALLEGRO_KEY_L},
        {"KEY_M", ALLEGRO_KEY_M},
        {"KEY_N", ALLEGRO_KEY_N},
        {"KEY_O", ALLEGRO_KEY_O},
        {"KEY_P", ALLEGRO_KEY_P},
        {"KEY_Q", ALLEGRO_KEY_Q},
        {"KEY_R", ALLEGRO_KEY_R},
        {"KEY_S", ALLEGRO_KEY_S},
        {"KEY_T", ALLEGRO_KEY_T},
        {"KEY_U", ALLEGRO_KEY_U},
        {"KEY_V", ALLEGRO_KEY_B},
        {"KEY_W", ALLEGRO_KEY_W},
        {"KEY_X", ALLEGRO_KEY_X},
        {"KEY_Y", ALLEGRO_KEY_Y},
        {"KEY_Z", ALLEGRO_KEY_Z},
        {"KEY_1", ALLEGRO_KEY_1},
        {"KEY_2", ALLEGRO_KEY_2},
        {"KEY_3", ALLEGRO_KEY_3},
        {"KEY_4", ALLEGRO_KEY_4},
        {"KEY_5", ALLEGRO_KEY_5},
        {"KEY_6", ALLEGRO_KEY_6},
        {"KEY_7", ALLEGRO_KEY_7},
        {"KEY_8", ALLEGRO_KEY_8},
        {"KEY_9", ALLEGRO_KEY_9},
        {"KEY_0", ALLEGRO_KEY_0},
        {"KEY_ENTER", ALLEGRO_KEY_ENTER},
        {"KEY_SPACE", ALLEGRO_KEY_SPACE}
};

struct OperandObject {};

struct OperandConstant : OperandObject {
    int value;
    explicit OperandConstant(int value) {
        this->value = value;
    };
};

struct OperandProperty : OperandObject {
    std::string spr;
    std::string prop;
    OperandProperty(std::string spr, std::string prop) {
        this->spr = std::move(spr);
        this->prop = std::move(prop);
    };
};

struct OperandString : OperandObject {
    std::string string;
    OperandString(std::string string) {
        this->string = std::move(string);
    };
};

struct OperandKey : OperandObject {
    int key;
    OperandKey(std::string keyStr) {
        this->key = (stringToKeyCode.find(keyStr) != stringToKeyCode.end() ? stringToKeyCode.at(keyStr) : -1);
    };
};

struct EventCondition {
    ConditionType type;
    std::pair<OperandType, OperandObject*> operand1;
    std::pair<OperandType, OperandObject*> operand2;
};

struct EventAction {
    ActionType type;
    std::pair<OperandType, OperandObject*> operand1;
    std::pair<OperandType, OperandObject*> operand2;
};

class Event {
public:
    std::vector<EventCondition> conditions = {};
    std::vector<EventAction> actions = {};

    static void test();
    static std::string to_string(EventCondition e);
    static std::string to_string(EventAction a);
    static std::string to_string(Event *ev);

    Event();
    static Event* decode(std::string eventString);
    bool evaluateCondition(EventCondition condition, Map *map);
    bool evaluateAction(EventAction action, Map *map);
    void doEvent(Map *map);

private:
    static std::map<std::string, ConditionType> conditionMap;
    static std::map<std::string, ActionType> actionMap;
};



#endif //FCWRPG_EVENT_H
