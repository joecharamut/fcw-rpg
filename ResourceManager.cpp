#include "ResourceManager.h"
#include "Util.h"

#include <allegro5/allegro_memfile.h>
#include <sys/stat.h>

Resource::Resource(ResourceLocation location, ResourceType type, byte *data, size_t size) {
    this->location = location;
    this->type = type;
    this->data = data;
    this->size = size;
}

ALLEGRO_FILE *Resource::openAllegroFile() {
    ALLEGRO_FILE *file = nullptr;

    file = al_open_memfile(data, size, "r");

    return file;
}

FILE *Resource::openFile() {
    FILE *file = nullptr;

    file = fmemopen(data, size, "r");

    return file;
}

std::map<ResourceLocation, Resource *> ResourceManager::resources;

void ResourceManager::registerResource(Resource *resource) {
    resources[resource->location] = resource;
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

Resource *ResourceManager::loadFileToResource(std::string filePath, std::string location) {
    Resource *resource;

    FILE *file = fopen(filePath.c_str(), "r");
    struct stat stat_buf;
    fstat(fileno(file), &stat_buf);
    size_t size = (size_t) stat_buf.st_size;
    byte *data = (byte *) calloc(sizeof(byte), size);
    fread(data, size, 1, file);

    resource = new Resource(
            ResourceLocation(location),
            ResourceType(Util::splitString(filePath, ".").back()),
            data, size
        );
    registerResource(resource);
    return resource;
}


