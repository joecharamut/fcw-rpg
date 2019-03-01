#include "ResourceFile.h"

#include <allegro5/allegro_memfile.h>

ALLEGRO_FILE *ResourceFile::openAllegroFile() {
    ALLEGRO_FILE *file = nullptr;

    file = al_open_memfile(data, size, "r");

    return file;
}

std::stringstream ResourceFile::openStream() {
    std::stringstream stream;
    for (int i = 0; i < size; i++) {
        stream << ((byte *) data)[i];
    }
    return stream;
}