#ifndef FCWRPG_RESOURCEMANAGER_H
#define FCWRPG_RESOURCEMANAGER_H

#include <string>
#include <map>
#include <allegro5/file.h>

#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>

typedef unsigned char byte;

class ResourceJSON {
public:
    std::map<std::string, std::string> resources;

    template <class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(resources)
        );
    }
};

class ResourceType {
public:
    std::string description;
    std::string extension;

    ResourceType(std::string description, std::string extension) : description(description), extension(extension) {};
    ResourceType(std::string extension) : ResourceType("", extension) {};
    ResourceType() : ResourceType("", "") {};
};

class ResourceLocation {
public:
    std::string location;

    ResourceLocation() = default;
    ResourceLocation(std::string locationString) : location(locationString) {};

    std::string getNamespace();
    std::string getPath();

    bool operator <(const ResourceLocation &r2) const {
        return (location < r2.location);
    }

    template <class Archive>
    void serialize(Archive &archive) {
        archive(
                CEREAL_NVP(location)
        );
    }
};

class Resource {
public:
    ResourceLocation location;
    ResourceType type;
    byte *data;
    size_t size;

    Resource(ResourceLocation location, ResourceType type, byte *data, size_t size);
    ~Resource() { free(data); };

    ALLEGRO_FILE *openAllegroFile();
    FILE *openFile();
};


class ResourceManager {
public:

    static Resource *loadFileToResource(std::string filePath, std::string location);
    static void registerResource(Resource *resource);
    static Resource *getResource(ResourceLocation location);
    static Resource *getResource(std::string location);

private:
    static std::map<ResourceLocation, Resource *> resources;
};


#endif //FCWRPG_RESOURCEMANAGER_H
