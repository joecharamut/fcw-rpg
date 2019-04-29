#ifndef FCWRPG_DATALOADER_H
#define FCWRPG_DATALOADER_H

#include <map>
#include <queue>
#include <ResourceFile.h>
#include <Registry.h>

enum Priority {
    PRIORITY_SPRITE,
    PRIORITY_OBJECT,
    PRIORITY_MAP,
    PRIORITY_DEFAULT
};

template<class T>
class PriorityObject {
    static_assert(std::is_pointer<T>::value, "PriorityObject T requires pointer");
public:
    Priority priority = PRIORITY_DEFAULT;
    T value = nullptr;

    PriorityObject() = default;
    PriorityObject(T v, Priority p) : priority(p), value(v) {}

    ~PriorityObject() {
        delete value;
    }

    friend bool operator<(const PriorityObject &a, const PriorityObject &b) {
        return a.priority > b.priority;
    }
};

class DataLoader {
public:
    static bool load();

private:
    static std::priority_queue<PriorityObject<ResourceFile *>> postProcessQueue;
    static Registry<ResourceFile *> fileRegistry;

    static std::map<std::string, Priority> toPriorityMap;
    static std::map<Priority, std::string> toStringMap;
    static std::map<std::string, bool(*)(std::string)> fileHandlers;
    static std::map<Priority, bool(*)(ResourceFile *)> fileProcessors;
    static void initMaps();

    static std::pair<unsigned char *, int> read_file(std::string path);
    static std::string getFileName(std::string path);
    static std::string getFileExtension(std::string path);

    // File Load Handlers
    static bool handle_resource(std::string path);
    static bool handle_basic_file(std::string path);
    static bool handle_json(std::string path);

    // File Processing Handlers
    static bool process_resource(PriorityObject<ResourceFile *> obj);
    static bool process_sprite(ResourceFile *file);
    static bool process_object(ResourceFile *file);
    static bool process_map(ResourceFile *file);
};


#endif //FCWRPG_DATALOADER_H
