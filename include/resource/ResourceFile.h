#include <utility>

#ifndef FCWRPG_RESOURCEFILE_H
#define FCWRPG_RESOURCEFILE_H

#include <cstdlib>
#include <allegro5/file.h>
#include <sstream>
#include <vector>

typedef unsigned char byte;

class ResourceFile {
public:
    void *data;
    size_t size;
    std::string path;

    ResourceFile(void *data, size_t size, std::string path) : data(data), size(size), path(std::move(path)) {};
    ResourceFile(void *data, size_t size) : ResourceFile(data, size, "") {};
    ~ResourceFile();

    static ResourceFile *loadFileToResource(std::string filePath);

    ALLEGRO_FILE *openAllegroFile();
    std::stringstream openStream();
};


#endif //FCWRPG_RESOURCEFILE_H
