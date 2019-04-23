#include <load/DataLoader.h>
#include <Log.h>
#include <Util.h>
#include <experimental/filesystem>
#include <sys/stat.h>
#include <cereal/archives/json.hpp>
#include <load/TypeJSON.h>

std::priority_queue<PriorityObject<ResourceFile *>> DataLoader::postProcessQueue;

bool DataLoader::load() {
    std::string dataPath = "resources/data";

    Log::debug("Loading data");
    long long int s_time = Util::getMilliTime();

    if (!std::experimental::filesystem::exists(dataPath)) {
        Log::error("Data path does not exist!");
        return false;
    }

    for (const auto &p : std::experimental::filesystem::recursive_directory_iterator(dataPath)) {
        if (std::experimental::filesystem::is_directory(p)) {
            continue;
        }

        std::string path = p.path().string();

        if (!handle_file(path)) {
            Log::errorf("Error handling resource %s", path.c_str());
            return false;
        } else {
            Log::verbosef("Processed resource %s successfully", path.c_str());
        }
    }

    Log::debugf("Data loaded successfully in %d ms", (int) (Util::getMilliTime() - s_time));

    Log::debug("Processing data");
    s_time = Util::getMilliTime();

    ResourceFile *file;
    while (!postProcessQueue.empty()) {
        file = postProcessQueue.top().value;
        postProcessQueue.pop();

        // TODO: processing
    }

    Log::debugf("Data processed successfully in %d ms", (int) (Util::getMilliTime() - s_time));

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

bool DataLoader::handle_file(std::string path) {
    std::string extension = Util::splitString(path, ".").back();

    if (fileHandlers.empty()) {
        initFileHandlers();
    }

    if (fileHandlers.count(extension)) {
        return fileHandlers.at(extension)(path);
    }

    Log::errorf("No handler registered for type %s", extension.c_str());
    return false;
}

std::map<std::string, bool(*)(std::string)> DataLoader::fileHandlers;
void DataLoader::initFileHandlers() {
    fileHandlers["png"] = DataLoader::handle_png;
    fileHandlers["json"] = DataLoader::handle_json;
    fileHandlers["ogg"] = DataLoader::handle_ogg;
}

bool DataLoader::handle_png(std::string path) {
    return false;
}

bool DataLoader::handle_ogg(std::string path) {
    return false;
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

    Priority pri;
    if (type == "sprite") {
        pri = PRIORITY_SPRITE;
    } else if (type == "object") {
        pri = PRIORITY_OBJECT;
    } else if (type == "map") {
        pri = PRIORITY_MAP;
    } else {
        Log::errorf("Error loading JSON type %s, at %s", type.c_str(), path.c_str());
        return false;
    }

    postProcessQueue.push(PriorityObject<ResourceFile *>(new ResourceFile(data, (size_t) size), pri));
    return true;
}
