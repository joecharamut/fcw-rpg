#ifndef FCWRPG_TYPEJSON_H
#define FCWRPG_TYPEJSON_H

#include <string>

#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>


class TypeJSON {
public:
    std::string type;

    TypeJSON() = default;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(type)
        );
    }
};

#endif //FCWRPG_TYPEJSON_H
