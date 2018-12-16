#include <utility>

#include <regex>
#include <iostream>

#include "Event.h"
#include "Util.h"
#include "Keyboard.h"
#include "Music.h"

std::map<std::string, ConditionType> Event::conditionMap =  {
        {"NE", NOT_EQUAL},
        {"EQ", EQUALS},
        {"GT", GREATER},
        {"LT", LESS},
        {"GE", GREATER_OR_EQUAL},
        {"LE", LESS_OR_EQUAL},
        {"DOWN", DOWN}
};

std::map<std::string, ActionType > Event::actionMap =  {
        {"SET", ACTION_SET}, {"PLAY", ACTION_PLAY}
};

Event::Event() = default;

std::string Event::to_string(EventCondition e) {
    std::string str;
    str += std::to_string(e.type) + ", ";
    str += "(" + std::to_string(e.operand1.first) + ":";
    if (e.operand1.first == TYPE_CONSTANT) {
        auto *o = (OperandConstant*) e.operand1.second;
        str += std::to_string(o->value) + ")";
    } else if (e.operand1.first == TYPE_PROPERTY) {
        auto *o = (OperandProperty*) e.operand1.second;
        str += o->spr + "::" + o->prop + ")";
    }
    str += ", ";
    str += "(" + std::to_string(e.operand2.first) + ":";
    if (e.operand2.first == TYPE_CONSTANT) {
        auto *o = (OperandConstant*) e.operand2.second;
        str += std::to_string(o->value) + ")";
    } else if (e.operand2.first == TYPE_PROPERTY) {
        auto *o = (OperandProperty*) e.operand2.second;
        str += o->spr + "::" + o->prop + ")";
    }
    return str;
}

std::string Event::to_string(EventAction a) {
    std::string str;
    str += std::to_string(a.type) + ", ";
    str += "(" + std::to_string(a.operand1.first) + ":";
    if (a.operand1.first == TYPE_CONSTANT) {
        auto *o = (OperandConstant*) a.operand1.second;
        str += std::to_string(o->value) + ")";
    } else if (a.operand1.first == TYPE_PROPERTY) {
        auto *o = (OperandProperty*) a.operand1.second;
        str += o->spr + "::" + o->prop + ")";
    }
    str += ", ";
    str += "(" + std::to_string(a.operand2.first) + ":";
    if (a.operand2.first == TYPE_CONSTANT) {
        auto *o = (OperandConstant*) a.operand2.second;
        str += std::to_string(o->value) + ")";
    } else if (a.operand2.first == TYPE_PROPERTY) {
        auto *o = (OperandProperty*) a.operand2.second;
        str += o->spr + "::" + o->prop + ")";
    }
    return str;
}

std::string Event::to_string(Event *ev) {
    std::string str;
    str += "[";
    for (EventCondition e : ev->conditions) {
        str += "{" + to_string(e) + "}, ";
    }
    str = str.substr(0, str.length()-3) + "] [";
    for (EventAction a : ev->actions) {
        str += "{" + to_string(a) + "}, ";
    }
    str = str.substr(0, str.length()-3) + "]";
    return str;
}

std::pair<OperandType, OperandObject*> makeOperand(const std::string &str) {
    OperandType type;
    OperandObject *object;
    if (Util::checkInt(str)) {
        type = TYPE_CONSTANT;
        object = new OperandConstant(std::stoi(str));
    } else if (str.find('.') != std::string::npos) {
        type = TYPE_PROPERTY;
        auto split = Util::splitString(str, ".");
        object = new OperandProperty(split[0], split[1]);
    } else if (str.substr(0, 4) == "KEY_") {
        type = TYPE_KEY;
        object = new OperandKey(str);
    } else {
        type = TYPE_STRING;
        object = new OperandString(str);
    }
    return std::make_pair(type, object);
}

Event* Event::decode(std::string eventString) {
    auto *event = new Event();

    auto eventParts = Util::splitString(std::move(eventString), "//");

    if (eventParts.size() < 2) {
        throw std::invalid_argument(
                "Invalid Event Format ("+std::to_string(eventParts.size())+" Parts Found, Expected 2 or 3)");
    }

    std::string conditionString = eventParts[0];
    std::string actionString = eventParts[1];
    std::string delay = (eventParts.size() > 2 ? eventParts[2] : "");

    //Util::log("Condition Part: " + conditionString, "Decode");

    auto conditionParts = Util::splitString(conditionString, ",");
    for (auto str : conditionParts) {
        str = str.substr(1, str.length()-2);
        auto split = Util::splitString(str, " ");

        int key = 0;
        EventCondition condition = {};
        for (const auto &part : split) {
            switch (key++) {
                case 0:
                    condition.operand1 = makeOperand(part);
                    break;
                case 1:
                    if (conditionMap.count(part)) {
                        condition.type = conditionMap.at(part);
                    } else {
                        throw std::invalid_argument("Operator "+part+" Invalid");
                    }
                    break;
                case 2:
                    condition.operand2 = makeOperand(part);
                    break;
                default:break;
            }
        }
        event->conditions.push_back(condition);
    }

    //Util::log("Action Part: "+actionString, "Decode");

    auto actionParts = Util::splitString(actionString, ",");
    for (auto str : actionParts) {
        str = str.substr(1, str.length()-2);
        auto split = Util::splitString(str, " ");
        int key = 0;
        EventAction action = {};
        for (const auto &part : split) {
            switch (key++) {
                case 0:
                    if (actionMap.count(part)) {
                        action.type = actionMap.at(part);
                    } else {
                        throw std::invalid_argument("Action "+part+" Invalid");
                    }
                    break;
                case 1:
                    action.operand1 = makeOperand(part);
                    break;
                case 2:
                    action.operand2 = makeOperand(part);
                    break;
                default:break;
            }
        }
        event->actions.push_back(action);
    }

    if (!delay.empty()) {
        if (Util::checkInt(delay)) {
            event->delay = std::stoi(delay);
        }
    }

    return event;
}

float getValue(std::pair<OperandType, OperandObject *> op, Map *map) {
    if (op.first == TYPE_CONSTANT) {
        return ((OperandConstant *) op.second)->value;
    } else if (op.first == TYPE_PROPERTY) {
        Sprite *sprite = map->getSpriteById(((OperandProperty *) op.second)->spr);
        std::string prop = ((OperandProperty *) op.second)->prop;
        if (!sprite) {
            return INT_MIN;
        }
        if (prop == "x") {
            return sprite->getX();
        } else if (prop == "y") {
            return sprite->getY();
        }
    } else if (op.first == TYPE_KEY) {
        Keycode key = ((OperandKey *) op.second)->key;
        if (key == -1) return INT_MIN;
        return key;
    } else if (op.first == TYPE_STRING) {
        std::string str = ((OperandString *) op.second)->string;
        if (str == "key") return 0;
        return -1;
    }
    return INT_MIN;
}

std::string getValueString(std::pair<OperandType, OperandObject *> op) {
    if (op.first == TYPE_STRING) {
        return ((OperandString *) op.second)->string;
    }
    return "";
}

bool setValue(std::pair<OperandType, OperandObject *> op1, std::pair<OperandType, OperandObject *> op2, Map *map) {
    if (op1.first == TYPE_PROPERTY) {
        float newValue = getValue(op2, map);
        Sprite *sprite = map->getSpriteById(((OperandProperty*) op1.second)->spr);
        std::string prop = ((OperandProperty*) op1.second)->prop;

        if (!sprite || newValue == INT_MIN) {
            return false;
        }
        if (prop == "x") {
            sprite->setX(newValue);
        } else if (prop == "y") {
            sprite->setY(newValue);
        }
        return true;
    }
    return false;
}

bool play(std::pair<OperandType, OperandObject *> op1, std::pair<OperandType, OperandObject *> op2, Map *map) {
    std::string key = getValueString(op1);
    std::string value = getValueString(op2);
    if (key.empty() || value.empty()) {
        return false;
    }
    if (key == "MUSIC") {
        if (map->music.find(value) != map->music.end()) {
            Music::playMusic(map->music.at(value));
            return true;
        }
    } else if (key == "SFX") {
        if (map->soundEffects.find(value) != map->soundEffects.end()) {
            Music::playMusic(map->soundEffects.at(value));
            return true;
        }
    }
    return false;
}

bool Event::evaluateCondition(EventCondition condition, Map *map) {
    float value1 = getValue(condition.operand1, map);
    float value2 = getValue(condition.operand2, map);
    if (value1 == INT_MIN || value2 == INT_MIN) return false;
    switch (condition.type) {
        case NOT_EQUAL:
            return value1 != value2;
        case EQUALS:
            return value1 == value2;
        case GREATER:
            return value1 > value2;
        case LESS:
            return value1 < value2;
        case GREATER_OR_EQUAL:
            return value1 >= value2;
        case LESS_OR_EQUAL:
            return value1 <= value2;
        case DOWN:
            return Keyboard::getKeyState((Keycode) value2);
        default:
            return false;
    }
}

bool Event::evaluateAction(EventAction action, Map *map) {
    if (action.type == ACTION_SET) {
        return setValue(action.operand1, action.operand2, map);
    } else if (action.type == ACTION_PLAY) {
        return play(action.operand1, action.operand2, map);
    }
    return false;
}

void Event::doEvent(Map *map) {
    if (timer > 0) {
        timer--;
        return;
    }
    long long int start = Util::getMilliTime();
    bool state = true;
    for (auto c : conditions) {
        state &= evaluateCondition(c, map);
    }
    if (state && timer == 0) {
        timer = delay;
        for (auto a : actions) {
            state &= evaluateAction(a, map);
        }
        if (state) {
            std::cout << "[DEBUG]: Event group executed successfully in " << std::to_string(Util::getMilliTime()-start) << "ms" << std::endl;
        } else {
            std::cout << "[DEBUG]: Event group failed in " << std::to_string(Util::getMilliTime()-start) << "ms" << std::endl;
        }
    }

}