#include "MapLoader.h"
#include "Util.h"
#include "Log.h"
#include "Engine.h"
#include "module/Registries.h"

#include <experimental/filesystem>
#include <archive/archive.h>
#include <archive/archive_entry.h>

std::map<std::string, Map *> MapLoader::mapList = {};

bool MapLoader::loadMaps() {
    // Create the list
    std::vector<std::string> maps = {};

    std::vector<std::pair<std::string, bool>> mapPaths = {};
    // Define the base path
    std::string mapPath = "resources/maps";
    // If the path doesn't exist, return empty list
    if (!std::experimental::filesystem::exists(mapPath)) {
        Log::error("Map path does not exist!");
        exit(1);
    }
    // For each file in the maps directory,
    for (const auto &p : std::experimental::filesystem::directory_iterator(mapPath)) {
        if (std::experimental::filesystem::is_directory(p)) {
            // and each file in that folder,
            for (const auto &p2 : std::experimental::filesystem::directory_iterator(p.path().string())) {
                // Get the filename/path
                std::string file = p2.path().string();
                // Fix Windows path issue (Replace \ with /)
                std::replace(file.begin(), file.end(), '\\', '/');
                // If it ends in .json
                if (file.find("map.json") != std::string::npos) {
                    // Assume unpacked map
                    /*std::string id;
                    if ((id = processUnpackedResources(file.substr(0, file.size() - 8))).empty()) {
                        return false;
                    } else {
                        maps.push_back(id);
                    }*/
                    mapPaths.emplace_back(file.substr(0, file.size() - 8), false);
                }
            }
        } else if (p.path().string().substr(p.path().string().length() - 4) == ".map") {
            // Assume .map packed file

            // Get the filename/path
            std::string file = p.path().string();
            // Fix Windows path issue (Replace \ with /)
            std::replace(file.begin(), file.end(), '\\', '/');
            // Load
            /*std::string id;
            if ((id = processPackedResources(file)).empty()) {
                return false;
            } else {
                maps.push_back(id);
            }*/
            mapPaths.emplace_back(file, true);
        }
    }

    Log::debugf("Discovered %d maps.", mapPaths.size());

    Engine::setLoadingProgress(1, 0);
    for (const auto &pair : mapPaths) {
        std::string id;
        if (pair.second) {
            // Packed Map
            if ((id = processPackedResources(pair.first)).empty()) {
                return false;
            }
        } else {
            // Unpacked Map
            if ((id = processUnpackedResources(pair.first)).empty()) {
                return false;
            }
        }
        maps.push_back(id);
        Engine::updateLoadingProgress(1, 1.0f/mapPaths.size());
    }

    for (const std::string &map : maps) {
        if (!processMap(map)) {
            Log::errorf("Error loading map %s", map.c_str());
            return false;
        }
    }

    return true;
}

bool MapLoader::processMap(std::string id) {
    long long int start = Util::getMilliTime();

    ResourceFile *spriteFile = Registries::resourceFileRegistry.get(id + ":_sprites");
    if (!spriteFile) return false;
    std::stringstream spriteStream = spriteFile->openStream();
    if (spriteStream.fail()) return false;
    cereal::JSONInputArchive spriteInput(spriteStream);
    std::vector<SpriteJSON> spriteList = {};
    spriteInput(cereal::make_nvp("sprites", spriteList));

    for (const auto &spr : spriteList) {
        auto *sprite = new Sprite(spr.id, spr.frames, spr.speed);
        Registries::spriteRegistry.put(sprite, id + ":" + sprite->id);
        Log::verbosef("Loaded Sprite %s:%s", id.c_str(), sprite->id.c_str());
    }

    std::vector<ResourceFile *> files = Registries::resourceFileRegistry.search(id + ":_room[0-9]+");
    for (ResourceFile *res : files) {
        std::stringstream inStream = res->openStream();
        std::string roomId;
        if (inStream.fail()) return false;

        cereal::JSONInputArchive input(inStream);
        RoomJSON roomJSON;
        input(cereal::make_nvp("data", roomJSON));
        roomId = roomJSON.id;
        Room *room = new Room(roomJSON.id, roomJSON.tileset, roomJSON.tilemap, roomJSON.objects);
        Registries::roomRegistry.put(room, id + ":" + roomId);
        Log::verbosef("Loaded Room %s:%s", id.c_str(), roomId.c_str());
    }

    ResourceFile *mapFile = Registries::resourceFileRegistry.get(id + ":_map");
    if (!mapFile) return false;
    std::stringstream mapStream = mapFile->openStream();
    if (mapStream.fail()) return false;

    cereal::JSONInputArchive inputArchive(mapStream);
    MapJSON mapJSON;
    inputArchive(cereal::make_nvp("data", mapJSON));
    Map *map = new Map(mapJSON.id, mapJSON.defaultRoom, mapJSON.rooms,
                       mapJSON.textsString, mapJSON.soundEffectsString, mapJSON.musicString);
    mapList[mapJSON.id] = map;

    long long int end = Util::getMilliTime();
    Log::debugf("Loaded Map %s (%d ms)", id.c_str(), end-start);
    return true;
}

std::string MapLoader::processUnpackedResources(std::string basePath) {
    std::string mapId;
    std::ifstream mapIdStream(basePath + "map.json", std::ios::binary);
    if (!mapIdStream.fail()) {
        cereal::JSONInputArchive inputArchive(mapIdStream);
        MapJSON json;
        inputArchive(cereal::make_nvp("data", json));
        mapId = json.id;
        mapIdStream.close();
    }
    if (mapId.empty()) return "";

    std::string dataPath = basePath + "data/";
    for (const auto &p : std::experimental::filesystem::recursive_directory_iterator(dataPath)) {
        if (!std::experimental::filesystem::is_directory(p)) {
            std::string filename = p.path().string();
            std::replace(filename.begin(), filename.end(), '\\', '/');
            filename = Util::splitString(filename, "/").back();
            std::string resourceName = mapId + ":" + Util::splitString(filename, ".").front();
            auto *res = ResourceFile::loadFileToResource(p.path().string());
            Registries::resourceFileRegistry.put(res, resourceName);
            Log::verbosef("Loaded Resource %s (%d bytes)", resourceName.c_str(), (int) res->size);
        }
    }

    for (const auto &p : std::experimental::filesystem::directory_iterator(basePath)) {
        if (!std::experimental::filesystem::is_directory(p)) {
            std::string filename = p.path().string();
            std::replace(filename.begin(), filename.end(), '\\', '/');
            filename = Util::splitString(filename, "/").back();

            if (filename.substr(filename.length() - 5) != ".json") continue;

            std::string resourceName = mapId + ":_" + Util::splitString(filename, ".").front();
            auto *res = ResourceFile::loadFileToResource(p.path().string());
            Registries::resourceFileRegistry.put(res, resourceName);
            Log::verbosef("Loaded Resource %s (%d bytes)", resourceName.c_str(), (int) res->size);
        }
    }

    return mapId;
}

std::string MapLoader::processPackedResources(std::string file) {
    struct archive *archive;
    struct archive_entry *entry;
    int ret;

    std::string mapId;
    bool mapIdLoaded = false;

    FILE *filePtr = fopen(file.c_str(), "rb");
    struct stat stat_buf {};
    fstat(fileno(filePtr), &stat_buf);
    auto size = (size_t) stat_buf.st_size;
    auto *data = (byte *) calloc(sizeof(byte), size);
    fread(data, size, 1, filePtr);

    archive = archive_read_new();
    archive_read_support_format_zip(archive);

    //ret = archive_read_open_filename(archive, file.c_str(), 0);
    ret = archive_read_open_memory(archive, data, size);
    if (ret != ARCHIVE_OK) return "";

    while (archive_read_next_header(archive, &entry) == ARCHIVE_OK) {
        auto entry_size = (size_t) archive_entry_size(entry);
        std::string path = std::string(archive_entry_pathname(entry));
        std::string filename = Util::splitString(path, "/").back();

        if (!mapIdLoaded) {
            if (path == "map.json") {
                auto *buf = (byte *) calloc(sizeof(byte), entry_size);
                auto length = (int) archive_read_data(archive, buf, entry_size);

                std::stringstream stream;
                for (int i = 0; i < length; i++) {
                    stream << buf[i];
                }

                if (!stream.fail()) {
                    cereal::JSONInputArchive inputArchive(stream);
                    MapJSON json;
                    inputArchive(cereal::make_nvp("data", json));
                    mapId = json.id;
                }
                if (mapId.empty()) {
                    free(buf);
                    archive_read_free(archive);
                    return "";
                }
                mapIdLoaded = true;

                // Reload archive
                archive_read_free(archive);
                archive = archive_read_new();
                archive_read_support_format_zip(archive);
                ret = archive_read_open_memory(archive, data, size);
                if (ret != ARCHIVE_OK) return "";
            }
        } else {
            // If not a directory
            if (entry_size != 0) {
                // Grab the extension
                std::string ext = "." + Util::splitString(filename, ".").back();
                // Create the buffer
                auto *buf = (byte *) calloc(sizeof(byte), entry_size);
                // Extract the data into the buffer
                auto length = (int) archive_read_data(archive, buf, entry_size);
                // Register the resource into the resource manager
                std::string resourceName;
                if (path.find('/') == std::string::npos && path.substr(path.length() - 5) == ".json") {
                    resourceName = mapId + ":_" + Util::splitString(filename, ".").front();
                } else if (path.find("data/") != std::string::npos) {
                    resourceName = mapId + ":" + Util::splitString(filename, ".").front();
                }
                if (!resourceName.empty()) {
                    auto *res = new ResourceFile(buf, (size_t) length);
                    Registries::resourceFileRegistry.put(res, resourceName);
                    Log::verbosef("Loaded Packed Resource %s (%d bytes)", resourceName.c_str(), length);
                } else {
                    free(buf);
                }
            }
        }
    }
    // Free the archive
    archive_read_free(archive);

    return mapId;
}

Map *MapLoader::getMap(std::string id) {
    if (mapList.count(id)) {
        return mapList.at(id);
    }
    return nullptr;
}

