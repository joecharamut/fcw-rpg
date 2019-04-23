#ifndef FCWRPG_DATALOADER_H
#define FCWRPG_DATALOADER_H

#include <map>
#include <queue>
#include <ResourceFile.h>

enum Priority {
    PRIORITY_SPRITE,
    PRIORITY_OBJECT,
    PRIORITY_MAP
};

template<class T>
class PriorityObject {
public:
    Priority priority;
    T value;

    PriorityObject() = default;
    PriorityObject(T v, Priority p) : priority(p), value(v) {}

    friend bool operator<(const PriorityObject &a, const PriorityObject &b) {
        return a.priority > b.priority;
    }
};

class DataLoader {
public:
    static bool load();

private:
    static std::priority_queue<PriorityObject<ResourceFile *>> postProcessQueue;

    static std::map<std::string, bool(*)(std::string)> fileHandlers;
    static void initFileHandlers();

    static std::pair<unsigned char *, int> read_file(std::string path);

    static bool handle_file(std::string path);
    static bool handle_png(std::string path);
    static bool handle_json(std::string path);
    static bool handle_ogg(std::string path);
};


#endif //FCWRPG_DATALOADER_H
