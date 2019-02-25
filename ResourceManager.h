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
    std::string path;
    std::string extension;

    ResourceType(std::string path, std::string extension) : path(path), extension(extension) {};
    ResourceType(std::string extension) : ResourceType("", extension) {};
    ResourceType() : ResourceType("", "") {};
};

class ResourceLocation {
public:
    std::string location;

    ResourceLocation() = default;
    ResourceLocation(std::string locationString) : location(locationString) {};

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
