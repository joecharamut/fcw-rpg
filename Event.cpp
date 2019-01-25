#include "Event.h"
#include "duktape.h"
#include "Main.h"
#include "Engine.h"
#include <cstdio>
#include <duktape-cpp/DuktapeCpp.h>

static duk_ret_t native_print(duk_context *ctx) {
    printf("%s\n", duk_to_string(ctx, 0));
    return 0;
}

class GameSprite {
private:
    Sprite *sprite;

public:
    GameSprite(Sprite *sprite) {
        this->sprite = sprite;
    }

    std::string id() const {
        return this->sprite->id;
    }

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<GameSprite, Sprite *>);
        i.property("id", &GameSprite::id);
    }
};

class GameContext {
public:
    GameContext() = default;

    std::string currentMap() const {
        return Engine::current_map->id;
    }
    std::string currentRoom() const {
        return Engine::current_map->current_room->id;
    }

    std::vector<std::string> rooms() const {
        std::vector<std::string> list = {};
        for (auto room : Engine::current_map->rooms) {
            list.push_back(room.first);
        }
        return list;
    }

    GameSprite player() const {
        return GameSprite(Engine::player);
    }

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<GameContext>);
        i.property("current_map", &GameContext::currentMap);
        i.property("current_room", &GameContext::currentRoom);

        i.property("rooms", &GameContext::rooms);
        i.property("player", &GameContext::player);
    }
};
DUK_CPP_DEF_CLASS_NAME(GameSprite);
DUK_CPP_DEF_CLASS_NAME(GameContext);

void Event::test() {
    duk::Context context;

    duk_push_c_function(context, native_print, 1);
    duk_put_global_string(context, "print");

    context.registerClass<GameContext>();
    context.registerClass<GameSprite>();
    //ctx.evalStringNoRes("print(new GameContext().player.id);");
}

void Event::eval(std::string str) {
    duk::Context context;

    duk_push_c_function(context, native_print, 1);
    duk_put_global_string(context, "print");

    context.registerClass<GameContext>();
    context.registerClass<GameSprite>();
    try {
        context.evalStringNoRes(str.c_str());
    } catch (duk::DuktapeException &ex) {
        std::cerr << ex.what() << std::endl;
    }
}
