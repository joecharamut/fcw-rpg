#ifndef FCWRPG_REGISTRIES_H
#define FCWRPG_REGISTRIES_H


#include <gui/Gui.h>
#include <ResourceFile.h>
#include <Room.h>
#include <Sprite.h>
#include <object/Object.h>
#include <Registry.h>

class Registries {
public:
    static Registry<ResourceFile *> resourceFileRegistry;
    static Registry<Room *> roomRegistry;
    static Registry<Sprite *> spriteRegistry;
    static Registry<Object *> objectRegistry;
    static Registry<Gui *> guiRegistry;
};


#endif //FCWRPG_REGISTRIES_H
