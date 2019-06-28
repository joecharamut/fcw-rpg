#ifndef FCWRPG_REGISTRIES_H
#define FCWRPG_REGISTRIES_H


#include <gui/Gui.h>
#include <resource/ResourceFile.h>
#include <Sprite.h>
#include <object/Object.h>
#include <resource/Registry.h>
#include <Map.h>

class Registries {
public:
    static Registry<ResourceFile *> resourceFileRegistry;
    static Registry<Sprite *> spriteRegistry;
    static Registry<Object *> objectRegistry;
    static Registry<Gui *> guiRegistry;
    static Registry<Map *> mapRegistry;

    static void unloadRegistries();
};


#endif //FCWRPG_REGISTRIES_H
