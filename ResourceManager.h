#ifndef FCWRPG_RESOURCEMANAGER_H
#define FCWRPG_RESOURCEMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <allegro5/file.h>

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

typedef unsigned char byte;

class ResourceType {
public:
    enum Type {
        TYPE_VOID,
        TYPE_DATA
    };

    Type type;
    std::string extension;
    ResourceType(Type type, std::string extension) : type(type), extension(extension) {};
    ResourceType(std::string extension) : ResourceType(TYPE_VOID, extension) {};

    ResourceType() : ResourceType(TYPE_VOID, "") {};
};

class ResourceLocation {
public:
    std::string location;

    ResourceLocation() = default;
    ResourceLocation(std::string locationString) : location(locationString) {};

    bool operator <(const ResourceLocation &r2) const {
        return (location < r2.location);
    }
};

class Resource {
public:
    ResourceLocation location;
    ResourceType type;
    void *data;
    size_t size;

    Resource(ResourceLocation location, ResourceType type, void *data, size_t size);
    ~Resource() { free(data); };

    ALLEGRO_FILE *openAllegroFile();
    std::stringstream openStream();
};


class ResourceManager {
public:

    static Resource *loadFileToResource(std::string filePath, std::string location);
    static Resource *registerResource(Resource *resource);

    static Resource *getResource(ResourceLocation location);
    static Resource *getResource(std::string location);

    static std::vector<Resource *> getResources(std::string pattern);

private:
    static std::map<ResourceLocation, Resource *> resources;
};


#endif //FCWRPG_RESOURCEMANAGER_H
