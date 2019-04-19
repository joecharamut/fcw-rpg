#ifndef FCWRPG_RESOURCEFILE_H
#define FCWRPG_RESOURCEFILE_H

#include <cstdlib>
#include <allegro5/file.h>
#include <sstream>

typedef unsigned char byte;

class ResourceFile {
public:
    void *data;
    size_t size;

    ResourceFile(void *data, size_t size) : data(data), size(size) {};
    ~ResourceFile();

    static ResourceFile *loadFileToResource(std::string filePath);

    ALLEGRO_FILE *openAllegroFile();
    std::stringstream openStream();
};


#endif //FCWRPG_RESOURCEFILE_H
