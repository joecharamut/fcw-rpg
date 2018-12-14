#include <utility>

#include <regex>
#include <iostream>

#include "Event.h"
#include "Util.h"

std::map<std::string, ConditionType> Event::conditionMap =  {
        {"NE", NOT_EQUAL},
        {"EQ", EQUALS},
        {"GT", GREATER},
        {"LT", LESS},
        {"GE", GREATER_OR_EQUAL},
        {"LE", LESS_OR_EQUAL}
};

std::map<std::string, ActionType > Event::actionMap =  {
        {"SET", ACTION_SET}
};

std::map<std::string, int> Event::stringToKeyCode = {
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
        {"KEY_ENTER", ALLEGRO_KEY_ENTER}
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

void Event::test() {
    std::string input = "{s_hat.x LT 64} AND {s_hat.y LT 64}//{SET s_hat.x 128},{SET s_hat.y 128}";
    Util::log("Input String: " + input, "Test");
    Event *dec;
    try {
        dec = decode(input);
    } catch (std::exception &e) {
        Util::log(e.what(), ERROR);
        exit(1);
    }
    Util::log("Output Event:", "Test");
    int i = 0;
    for (EventCondition e : dec->conditions) {
        Util::log(to_string(e), "Condition "+std::to_string(++i));
    }
    i = 0;
    for (EventAction a : dec->actions) {
        Util::log(to_string(a), "Action "+std::to_string(++i));
    }
    //Util::log("Input Event: (Prev.)", "Test");
    //Util::log("Output String: ", "Test");
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
    } else {
        type = TYPE_STRING;
        object = new OperandString(str);
    }
    return std::make_pair(type, object);
}

Event* Event::decode(std::string eventString) {
    auto *event = new Event();
    std::regex rCondition("{[^{}]*}", std::regex_constants::basic);

    auto eventParts = Util::splitString(std::move(eventString), "//");

    if (eventParts.size() != 2) {
        throw std::invalid_argument(
                "Invalid Event Format ("+std::to_string(eventParts.size())+" Parts Found, Expected 2)");
    }

    std::string conditionString = eventParts[0];
    std::string actionString = eventParts[1];

    //Util::log("Condition Part: " + conditionString, "Decode");

    auto iter = std::sregex_iterator(conditionString.begin(), conditionString.end(), rCondition);
    for (auto i = iter; i != std::sregex_iterator(); ++i) {
        std::smatch match = *i;
        std::string str = match.str();
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

    return event;
}

float getValue(std::pair<OperandType, OperandObject *> op, Map *map) {
    if (op.first == TYPE_CONSTANT) {
        return ((OperandConstant*) op.second)->value;
    } else if (op.first == TYPE_PROPERTY) {
        Sprite *sprite = map->getSpriteById(((OperandProperty*) op.second)->spr);
        std::string prop = ((OperandProperty*) op.second)->prop;
        if (!sprite) {
            return INT_MIN;
        }
        if (prop == "x") {
            return sprite->getX();
        } else if (prop == "y") {
            return sprite->getY();
        }
    }
    return INT_MIN;
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
        default:
            return false;
    }
}

bool Event::evaluateAction(EventAction action, Map *map) {
    if (action.type == ACTION_SET) {
        return setValue(action.operand1, action.operand2, map);
    }
    return false;
}

void Event::doEvent(Map *map) {
    long long int start = Util::getMilliTime();
    bool state = true;
    for (auto c : conditions) {
        state &= evaluateCondition(c, map);
    }
    if (state) {
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