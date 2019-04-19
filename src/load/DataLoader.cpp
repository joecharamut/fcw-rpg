#include <load/DataLoader.h>
#include <Log.h>
#include <Util.h>
#include <experimental/filesystem>

bool DataLoader::load() {
    std::string dataPath = "resources/data";

    if (!std::experimental::filesystem::exists(dataPath)) {
        Log::error("Data path does not exist!");
        return false;
    }

    for (const auto &p : std::experimental::filesystem::recursive_directory_iterator(dataPath)) {
        if (std::experimental::filesystem::is_directory(p)) {
            continue;
        }

        if (!handle_file(p)) {
            Log::errorf("Error handling resource %s", p.path().c_str());
            return false;
        }
    }

    return true;
}

std::map<std::string, bool(*)(std::string)> DataLoader::handlers;

void DataLoader::init_handlers() {
    handlers["png"] = DataLoader::handle_png;
}

bool DataLoader::handle_file(std::experimental::filesystem::directory_entry p) {
    std::string path = p.path().string();
    std::string extension = Util::splitString(path, ".").back();

    if (handlers.empty()) {
        init_handlers();
    }

    if (handlers.count(extension)) {
        handlers.at(extension)(path);
    } else {
        Log::errorf("No handler registered for type %s", extension.c_str());
        return false;
    }

    return false;
}

bool DataLoader::handle_png(std::string path) {
    return false;
}
