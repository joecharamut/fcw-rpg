#include "MapLoader.h"
#include "Util.h"
#include "Log.h"
#include "ResourceManager.h"

#include <experimental/filesystem>
#include <archive/archive.h>
#include <archive/archive_entry.h>

std::map<std::string, Map *> MapLoader::mapList;

// Load a list of all maps
void MapLoader::loadMaps() {
    // Create the list
    std::vector<std::string> maps = {};
    // Define the base path
    std::string mapPath = "resources/maps";
    // If the path doesn't exist, return empty list
    if (!std::experimental::filesystem::exists(mapPath)) {
        return;
    }
    // For each file in the maps directory,
    for (const auto &p : std::experimental::filesystem::directory_iterator(mapPath)) {
        if (std::experimental::filesystem::is_directory(p)) {
            // and each file in that folder,
            for (const auto &p2 : std::experimental::filesystem::directory_iterator(p.path().string())) {
                // Get the filename/path
                std::string file = p2.path().string();
                // Fix Windows being mega gay (Replace \ with /)
                std::replace(file.begin(), file.end(), '\\', '/');
                // If it ends in .json
                if (file.find("map.json") != std::string::npos) {
                    // Assume unpacked map
                    loadUnpackedMap(file.substr(0, file.size() - 8));
                }
            }
        } else if (Util::splitString(p.path().string(), ".").back() == "map") {
            // Assume .map packed file

            // Get the filename/path
            std::string file = p.path().string();
            // Fix Windows being mega gay (Replace \ with /)
            std::replace(file.begin(), file.end(), '\\', '/');
            // Load
            loadPackedMap(file);
        }
    }
}

bool MapLoader::loadUnpackedMap(std::string basePath) {
    // Get system time (For load time calculation)
    long long int start = Util::getMilliTime();
    // File input stream
    std::ifstream resourceStream(basePath + "resources.json", std::ios::binary);
    if (!resourceStream.fail()) {
        cereal::JSONInputArchive input(resourceStream);
        ResourceJSON resourceJSON;
        input(cereal::make_nvp("data", resourceJSON));
        for (auto &resource : resourceJSON.resources) {
            auto *res = ResourceManager::loadFileToResource(basePath + resource.path, resource.location);
            printf("Loaded Resource %s (%d)\n", res->location.location.c_str(), (int) res->size);
        }
    }

    std::ifstream mapStream(basePath + "map.json", std::ios::binary);
    if (!mapStream.fail()) {
        cereal::JSONInputArchive inputArchive(mapStream);
        MapJSON json;
        inputArchive(cereal::make_nvp("mapdata", json));
        Map *m = new Map(json.id, json.defaultRoom, json.rooms, json.textsString,
                json.soundEffectsString, json.musicString);
        mapList[json.id] = m;
        long long int end = Util::getMilliTime();
        Log::info("Loaded Map " + json.id + " (" + std::to_string(end-start) + " ms)");
        return true;
    }
    return false;
}

bool MapLoader::loadPackedMap(std::string file) {
    long long int start = Util::getMilliTime();

    struct archive *archive;
    struct archive_entry *entry;
    int ret;

    archive = archive_read_new();
    archive_read_support_format_zip(archive);

    ret = archive_read_open_filename(archive, file.c_str(), 0);
    if (ret != ARCHIVE_OK) return false;

    bool resourcesLoaded = false;
    std::vector<std::string> resourceLocations;
    std::vector<std::string> resourcePaths;
    std::string mapFileId;

    while (archive_read_next_header(archive, &entry) == ARCHIVE_OK) {
        auto entry_size = (size_t) archive_entry_size(entry);

        // If not a directory
        if (entry_size != 0) {
            // Get the path
            std::string path = std::string(archive_entry_pathname(entry));
            if (!resourcesLoaded && path.find("resources.json") != std::string::npos) {
                // If we still don't have the resources, and it's the resources file, read the file
                byte *buf = (byte *) calloc(sizeof(byte), entry_size);
                int length = (int) archive_read_data(archive, buf, entry_size);

                // Check for errors
                if (length == ARCHIVE_FATAL) {
                    return false; // Fatal Error
                } else if (length == 0) {
                    return false; // Unexpected EOF
                }
                // Convert buffer to a StringStream
                std::stringstream ss;
                for (int i = 0; i < length; i++) {
                    ss << buf[i];
                }
                // Decode stream JSON data
                cereal::JSONInputArchive inputArchive(ss);
                ResourceJSON resourceJSON;
                inputArchive(cereal::make_nvp("data", resourceJSON));

                // Store requested resources
                for (auto &res : resourceJSON.resources) {
                    if (res.location.find(":mapfile") != std::string::npos) {
                        mapFileId = res.location;
                    }
                    resourceLocations.push_back(res.location);
                    resourcePaths.push_back(res.path);
                }

                // Set loaded
                resourcesLoaded = true;

                // Free the archive
                archive_read_free(archive);

                // Reload it
                archive = archive_read_new();
                archive_read_support_format_zip(archive);
                ret = archive_read_open_filename(archive, "resources/pack_test.map", 0);
                if (ret != ARCHIVE_OK) return false;

                // Free the resources json buffer
                free(buf);
            } else if (resourcesLoaded) {
                // If we are ready to load in the files, do it
                // For each resource
                for (int i = 0; i < resourcePaths.size(); i++) {
                    // If this archive entry is the right one
                    if (path.find(resourcePaths[i]) != std::string::npos) {
                        // Grab the extension
                        std::string ext = "." + Util::splitString(resourcePaths[i], ".").back();
                        // Create the buffer
                        byte *buf = (byte *) calloc(sizeof(byte), entry_size);
                        // Extract the data into the buffer
                        int length = (int) archive_read_data(archive, buf, entry_size);
                        // Register the resource into the resource manager
                        ResourceManager::registerResource(new Resource(
                                ResourceLocation(resourceLocations[i]),
                                ResourceType(ext),
                                buf, (size_t) length
                        ));
                        printf("Loaded Resource %s (%d)\n", resourceLocations[i].c_str(), (int) length);
                    }
                }
            }
        }
    }
    // Free the archive
    archive_read_free(archive);

    Resource *mapFile = ResourceManager::getResource(mapFileId);
    if (!mapFile) return false;

    std::stringstream mapStream = mapFile->openStream();
    std::cout << mapStream.str() << std::endl;

    cereal::JSONInputArchive inputArchive(mapStream);
    MapJSON json;
    inputArchive(cereal::make_nvp("mapdata", json));

    Map *m = new Map(json.id, json.defaultRoom, json.rooms, json.textsString,
            json.soundEffectsString, json.musicString);
    mapList[json.id] = m;
    long long int end = Util::getMilliTime();
    Log::info("Loaded Map " + json.id + " (" + std::to_string(end-start) + " ms)");

    return true;
}

Map *MapLoader::getMap(std::string id) {
    if (mapList.count(id) > 0) {
        return mapList.at(id);
    }
    return nullptr;
}


