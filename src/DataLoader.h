#ifndef FCWRPG_DATALOADER_H
#define FCWRPG_DATALOADER_H


#include <experimental/filesystem>
#include <map>

class DataLoader {
public:
    static bool load();

private:
    static std::map<std::string, bool(*)(std::string)> handlers;
    static void init_handlers();

    static bool handle_file(std::experimental::filesystem::directory_entry p);
    static bool handle_png(std::string path);
};


#endif //FCWRPG_DATALOADER_H
