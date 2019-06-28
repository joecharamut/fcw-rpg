#include <module/Registries.h>

Registry<ResourceFile *>    Registries::resourceFileRegistry;
Registry<Sprite *>          Registries::spriteRegistry;
Registry<Object *>          Registries::objectRegistry;
Registry<Gui *>             Registries::guiRegistry;
Registry<Map *>             Registries::mapRegistry;

void Registries::unloadRegistries() {
    // Unload ResFiles
    for (auto f : resourceFileRegistry) {
        delete f.second;
    }

    // Unload Sprites
    for (auto s : spriteRegistry) {
        s.second->unload();
        delete s.second;
    }

    // Unload Objects
    for (auto o : objectRegistry) {
        delete o.second;
    }

    // Unload GUIs
    for (auto g : guiRegistry) {
        g.second->unload();
        delete g.second;
    }

    // Unload Maps
    for (auto m : mapRegistry) {
        m.second->unload();
        delete m.second;
    }
}
