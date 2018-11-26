#include <utility>


#ifndef FCWRPG_EVENT_H
#define FCWRPG_EVENT_H

#include <vector>
#include <string>
#include <map>


enum ConditionType {
    NOT_EQUAL, EQUALS, GREATER, LESS, GREATER_OR_EQUAL, LESS_OR_EQUAL
};

enum OperandType {
    CONSTANT, PROPERTY
};

enum ActionType {
    SET
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
    static std::string encode(Event* event);
private:
    static std::map<std::string, ConditionType> conditionMap;
    static std::map<std::string, ActionType> actionMap;
};


#endif //FCWRPG_EVENT_H
