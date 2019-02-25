#include "ResourceManager.h"
#include "Util.h"
#include "Log.h"

#include <allegro5/allegro_memfile.h>
#include <sys/stat.h>
#include <sstream>
#include <regex>

Resource::Resource(ResourceLocation location, ResourceType type, void *data, size_t size) {
    this->location = location;
    this->type = type;
    this->data = data;
    this->size = size;
}

std::map<ResourceLocation, Resource *> ResourceManager::resources;

Resource *ResourceManager::registerResource(Resource *resource) {
    if (resources.count(resource->location) > 0) {
        Log::errorf("Resource at location %s already exists!", resource->location.location.c_str());
        return resource;
    }
    resources[resource->location] = resource;
    return resource;
}

Resource *ResourceManager::loadFileToResource(std::string filePath, std::string location) {
    Resource *resource;

    FILE *file = fopen(filePath.c_str(), "rb");
    struct stat stat_buf;
    fstat(fileno(file), &stat_buf);
    size_t size = (size_t) stat_buf.st_size;
    byte *data = (byte *) calloc(sizeof(byte), size);
    size_t read = fread(data, size, 1, file);

    resource = new Resource(
            ResourceLocation(location),
            ResourceType(filePath, "." + Util::splitString(filePath, ".").back()),
            data, size
    );
    registerResource(resource);
    return resource;
}

ALLEGRO_FILE *Resource::openAllegroFile() {
    ALLEGRO_FILE *file = nullptr;

    file = al_open_memfile(data, size, "r");

    return file;
}

std::stringstream Resource::openStream() {
    std::stringstream stream;
    for (int i = 0; i < size; i++) {
        stream << ((byte *) data)[i];
    }
    return stream;
}

Resource *ResourceManager::getResource(ResourceLocation location) {
    Resource *resource = nullptr;

    if (resources.count(location) != 0) {
        resource = resources[location];
    }

    return resource;
}

Resource *ResourceManager::getResource(std::string location) {
    return getResource(ResourceLocation(location));
}

std::vector<Resource *> ResourceManager::getResources(std::string pattern) {
    std::vector<Resource *> results;
    std::regex regex(pattern);

    for (const auto &p : resources) {
        Resource *res = p.second;
        if (std::regex_match(res->location.location, regex)) {
            results.push_back(res);
        }
    }

    return results;
}
