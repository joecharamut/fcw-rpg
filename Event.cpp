#include <utility>

#include <regex>
#include <stdexcept>
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
        {"SET", SET}
};

Event::Event() = default;

std::string Event::to_string(EventCondition e) {
    std::string str;
    str += std::to_string(e.type) + ", ";
    str += "(" + std::to_string(e.operand1.first) + ":";
    if (e.operand1.first == CONSTANT) {
        auto *o = (OperandConstant*) e.operand1.second;
        str += std::to_string(o->value) + ")";
    } else if (e.operand1.first == PROPERTY) {
        auto *o = (OperandProperty*) e.operand1.second;
        str += o->spr + "::" + o->prop + ")";
    }
    str += ", ";
    str += "(" + std::to_string(e.operand2.first) + ":";
    if (e.operand2.first == CONSTANT) {
        auto *o = (OperandConstant*) e.operand2.second;
        str += std::to_string(o->value) + ")";
    } else if (e.operand2.first == PROPERTY) {
        auto *o = (OperandProperty*) e.operand2.second;
        str += o->spr + "::" + o->prop + ")";
    }
    return str;
}

std::string Event::to_string(EventAction a) {
    std::string str;
    str += std::to_string(a.type) + ", ";
    str += "(" + std::to_string(a.operand1.first) + ":";
    if (a.operand1.first == CONSTANT) {
        auto *o = (OperandConstant*) a.operand1.second;
        str += std::to_string(o->value) + ")";
    } else if (a.operand1.first == PROPERTY) {
        auto *o = (OperandProperty*) a.operand1.second;
        str += o->spr + "::" + o->prop + ")";
    }
    str += ", ";
    str += "(" + std::to_string(a.operand2.first) + ":";
    if (a.operand2.first == CONSTANT) {
        auto *o = (OperandConstant*) a.operand2.second;
        str += std::to_string(o->value) + ")";
    } else if (a.operand2.first == PROPERTY) {
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
        type = CONSTANT;
        object = new OperandConstant(std::stoi(str));
    } else {
        type = PROPERTY;
        auto split = Util::splitString(str, ".");
        object = new OperandProperty(split[0], split[1]);
    }
    return std::make_pair(type, object);
}

Event* Event::decode(std::string eventString) {
    auto *event = new Event();
    std::regex rCondition("{[^{}]*}", std::regex_constants::basic);
    std::regex rOperation("\\} (AND|OR) \\{", std::regex_constants::extended);

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

    conditionString = eventParts[0];
    auto ops = std::sregex_iterator(conditionString.begin(), conditionString.end(), rOperation);
    for (auto i = ops; i != std::sregex_iterator(); ++i) {
        std::smatch match = *i;
        std::string str = match.str();
        str = str.substr(2, str.length() - 3);
        //Util::log(str);
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