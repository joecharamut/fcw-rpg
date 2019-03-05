#ifndef FCWRPG_OBJECT_H
#define FCWRPG_OBJECT_H

#include "../Sprite.h"

enum ObjectType {
    TYPE_NONE,
    TYPE_CONTAINER,
    TYPE_SIGN
};

class Object {
public:
    Sprite *sprite;
    ObjectType type;

    virtual void onInteract();
    virtual void onCollide();
};

#endif //FCWRPG_OBJECT_H
