#ifndef FCWRPG_JSONTYPE_H
#define FCWRPG_JSONTYPE_H

#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>


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

#endif //FCWRPG_JSONTYPE_H
