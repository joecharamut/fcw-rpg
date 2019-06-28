#include <allegro5/allegro_memfile.h>
#include <sys/stat.h>

#include <resource/ResourceFile.h>

ResourceFile *ResourceFile::loadFileToResource(std::string filePath) {
    ResourceFile *resource;

    FILE *file = fopen(filePath.c_str(), "rb");
    struct stat stat_buf {};
    fstat(fileno(file), &stat_buf);
    auto size = (size_t) stat_buf.st_size;
    auto *data = (byte *) calloc(sizeof(byte), size);
    fread(data, size, 1, file);
    resource = new ResourceFile(data, size);
    fclose(file);

    return resource;
}

ResourceFile::~ResourceFile() {
    free(data);
}

ALLEGRO_FILE *ResourceFile::openAllegroFile() {
    ALLEGRO_FILE *file = nullptr;

    file = al_open_memfile(data, size, "r");

    return file;
}

std::stringstream ResourceFile::openStream() {
    std::stringstream stream;
    for (int i = 0; i < (int)size; i++) {
        stream << ((byte *) data)[i];
    }
    return stream;
}
