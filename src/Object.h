#ifndef FCWRPG_OBJECT_H
#define FCWRPG_OBJECT_H

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>

class Test {
public:
    static void test();
};

class Object {
public:
    virtual void type() = 0;
};

class Container : public Object {
public:
    std::vector<std::string> contents;
    void type();

    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(contents));
    }
};

class Sign : public Object {
public:
    std::string text;
    void type();

    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(text));
    }
};

CEREAL_REGISTER_TYPE_WITH_NAME(Container, "Container");
CEREAL_REGISTER_POLYMORPHIC_RELATION(Object, Container);

CEREAL_REGISTER_TYPE_WITH_NAME(Sign, "Sign");
CEREAL_REGISTER_POLYMORPHIC_RELATION(Object, Sign);

#endif //FCWRPG_OBJECT_H
