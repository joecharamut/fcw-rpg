#ifndef FCWRPG_REGISTRIES_H
#define FCWRPG_REGISTRIES_H


#include <gui/Gui.h>
#include <ResourceFile.h>
#include <Sprite.h>
#include <object/Object.h>
#include <Registry.h>
#include <Map.h>

class Registries {
public:
    static Registry<ResourceFile *> resourceFileRegistry;
    static Registry<Sprite *> spriteRegistry;
    static Registry<Object *> objectRegistry;
    static Registry<Gui *> guiRegistry;
    static Registry<Map *> mapRegistry;
};


#endif //FCWRPG_REGISTRIES_H
