#include <utility>

#include <loader/DataLoader.h>
#include <loader/json/TypeJSON.h>
#include <Log.h>
#include <Util.h>
#include <experimental/filesystem>
#include <sys/stat.h>
#include <cereal/archives/json.hpp>
#include <loader/json/SpriteJSON.h>
#include <module/Registries.h>
#include <loader/json/MapJSON.h>
#include <MapLoader.h>
#include <loader/json/ObjectJSON.h>

std::priority_queue<PriorityObject<ResourceFile *>> DataLoader::postProcessQueue;

bool DataLoader::load() {
    std::string dataPath = "resources/data";

    Log::debug("Loading data");
    long long int s_time = Util::getMilliTime();

    if (!std::experimental::filesystem::exists(dataPath)) {
        Log::error("Data path does not exist!");
        return false;
    }

    initMaps();

    for (const auto &p : std::experimental::filesystem::recursive_directory_iterator(dataPath)) {
        if (std::experimental::filesystem::is_directory(p)) {
            continue;
        }

        std::string path = p.path().string();
        std::replace(path.begin(), path.end(), '\\', '/');

        if (!handle_resource(path)) {
            Log::errorf("Error handling resource %s", path.c_str());
            return false;
        } else {
            Log::verbosef("Loaded resource %s successfully", path.c_str());
        }
    }

    Log::debugf("Data loaded successfully in %d ms", (int) (Util::getMilliTime() - s_time));

    Log::debug("Processing data");
    s_time = Util::getMilliTime();

    PriorityObject<ResourceFile *> obj {};
    while (!postProcessQueue.empty()) {
        obj = postProcessQueue.top();
        postProcessQueue.pop();

        if (!process_resource(obj)) {
            Log::errorf("Error processing resource %s", obj.value->path.c_str());
            return false;
        } else {
            Log::verbosef("Processed resource %s successfully", obj.value->path.c_str());
        }
    }

    Log::debugf("Data processed successfully in %d ms", (int) (Util::getMilliTime() - s_time));

    Log::debug("Validating data");
    s_time = Util::getMilliTime();

    // TODO: Implement this

    Log::debugf("Data validated successfully in %d ms", (int) (Util::getMilliTime() - s_time));

    return true;
}

std::pair<unsigned char *, int> DataLoader::read_file(std::string path) {
    FILE *file = fopen(path.c_str(), "rb");
    struct stat stat_buf {};
    fstat(fileno(file), &stat_buf);
    auto size = (size_t) stat_buf.st_size;
    auto *data = (unsigned char *) calloc(sizeof(unsigned char), size);
    fread(data, size, 1, file);
    fclose(file);

    return {data, (int) size};
}

std::string DataLoader::getFileName(std::string path) {
    std::string name = Util::splitString(path, "/").back();
    return name.substr(0, name.length() - (getFileExtension(path).length() + 1));
}

std::string DataLoader::getFileExtension(std::string path) {
    return Util::splitString(std::move(path), ".").back();
}

bool DataLoader::handle_resource(std::string path) {
    std::string extension = getFileExtension(path);

    if (fileHandlers.count(extension)) {
        return fileHandlers.at(extension)(path);
    }

    Log::errorf("No handler registered for type %s", extension.c_str());
    return false;
}

bool DataLoader::process_resource(PriorityObject<ResourceFile *> obj) {
    if (fileProcessors.count(obj.priority)) {
        return fileProcessors.at(obj.priority)(obj.value);
    }

    Log::errorf("No processor registered for type %s", toStringMap[obj.priority].c_str());
    return false;
}

std::map<std::string, Priority> DataLoader::toPriorityMap;
std::map<Priority, std::string> DataLoader::toStringMap;
std::map<std::string, bool(*)(std::string)> DataLoader::fileHandlers;
std::map<Priority, bool(*)(ResourceFile *)> DataLoader::fileProcessors;
void DataLoader::initMaps() {
    // str -> pri
    toPriorityMap["sprite"] = PRIORITY_SPRITE;
    toPriorityMap["object"] = PRIORITY_OBJECT;
    toPriorityMap["map"] = PRIORITY_MAP;

    // pri -> str
    toStringMap[PRIORITY_SPRITE] = "sprite";
    toStringMap[PRIORITY_OBJECT] = "object";
    toStringMap[PRIORITY_MAP] = "map";

    // str -> handler
    fileHandlers["png"] = DataLoader::handle_basic_file;
    fileHandlers["ogg"] = DataLoader::handle_basic_file;
    fileHandlers["json"] = DataLoader::handle_json;

    // pri -> processor
    fileProcessors[PRIORITY_SPRITE] = DataLoader::process_sprite;
    fileProcessors[PRIORITY_OBJECT] = DataLoader::process_object;
    fileProcessors[PRIORITY_MAP] = DataLoader::process_map;
}

bool DataLoader::handle_basic_file(std::string path) {
    unsigned char *data;
    size_t size;
    std::tie(data, size) = read_file(path);
    Registries::resourceFileRegistry.put(new ResourceFile(data, size), getFileName(path));

    return true;
}

bool DataLoader::handle_json(std::string path) {
    unsigned char *data;
    int size;
    std::tie(data, size) = read_file(path);

    std::stringstream stream;
    for (int i = 0; i < size; i++) {
        stream << data[i];
    }

    cereal::JSONInputArchive input(stream);
    TypeJSON typeJSON;
    input(cereal::make_nvp("data", typeJSON));
    std::string type = typeJSON.type;

    if (toPriorityMap.count(type)) {
        postProcessQueue.push(
                PriorityObject<ResourceFile *>(new ResourceFile(data, (size_t) size, path), toPriorityMap.at(type)));
        return true;
    }

    Log::errorf("Error loading JSON type %s, at %s", type.c_str(), path.c_str());
    return false;
}

bool DataLoader::process_sprite(ResourceFile *file) {
    std::stringstream stream = file->openStream();
    cereal::JSONInputArchive input(stream);
    SpriteJSON spriteJSON;
    input(cereal::make_nvp("data", spriteJSON));

    Sprite *spr = spriteJSON.construct();
    Registries::spriteRegistry.put(spr, getFileName(file->path));
    return true;
}

bool DataLoader::process_object(ResourceFile *file) {
    std::stringstream stream = file->openStream();
    cereal::JSONInputArchive input(stream);
    ObjectJSON objectJSON;
    input(cereal::make_nvp("data", objectJSON));

    Object *obj = objectJSON.construct();
    Registries::objectRegistry.put(obj, getFileName(file->path));
    return true;
}

bool DataLoader::process_map(ResourceFile *file) {
    std::stringstream stream = file->openStream();
    cereal::JSONInputArchive input(stream);
    MapJSON mapJSON;
    input(cereal::make_nvp("data", mapJSON));

    Map *map = mapJSON.construct();
    MapLoader::mapList[map->id] = map;
    return true;
}

