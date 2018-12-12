
#ifndef FCWRPG_EVENT_H
#define FCWRPG_EVENT_H

#include <utility>
#include <vector>
#include <string>
#include <map>

#include "Map.h"
class Map;

enum ConditionType {
    NOT_EQUAL, EQUALS, GREATER, LESS, GREATER_OR_EQUAL, LESS_OR_EQUAL
};

enum OperandType {
    TYPE_CONSTANT, TYPE_PROPERTY
};

enum ActionType {
    ACTION_SET
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
