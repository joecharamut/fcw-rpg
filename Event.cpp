#include <utility>

#include "Event.h"
#include "duktape.h"
#include "Main.h"
#include "Engine.h"
#include <cstdio>
#include <duktape-cpp/DuktapeCpp.h>
#include <cppcodec/base64_rfc4648.hpp>

static duk_ret_t native_print(duk_context *ctx) {
    printf("%s\n", duk_to_string(ctx, 0));
    return 0;
}

struct GameSprite {
    #define getSprite(x) Engine::current_map->getSpriteById(x)

    GameSprite() = default;
    GameSprite(std::string id) {
        _id = id;
        spr = getSprite(_id);
        x = spr->getX();
        y = spr->getY();
    }

    Sprite *spr;

    std::string _id;
    float x;
    float y;

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<GameSprite, std::string>);
    }
};

namespace duk {
    template <>
    struct Type<GameSprite> {
        static void push(duk::Context &ctx, GameSprite const &val) {
            duk_push_object(ctx);

            duk_push_string(ctx, val._id.c_str());
            duk_put_prop_string(ctx, -2, "id");

            duk_push_number(ctx, val.x);
            duk_put_prop_string(ctx, -2, "x");

            duk_push_number(ctx, val.y);
            duk_put_prop_string(ctx, -2, "y");
        }

        static void get(duk::Context &ctx, GameSprite &val, int index) {
            duk_get_prop_string(ctx, index, "id");
            std::string id = duk_get_string(ctx, -1);
            duk_pop(ctx);

            val = GameSprite(id);
        }

        static constexpr bool isPrimitive() { return true; }
    };
}

DUK_CPP_DEF_CLASS_NAME(GameSprite);

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
        return GameSprite(Engine::player->id);
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
DUK_CPP_DEF_CLASS_NAME(GameContext);

Event::Event(std::string encodedEvent) {
    using base64 = cppcodec::base64_rfc4648;
    std::vector<uint8_t> decoded = base64::decode(encodedEvent);
    std::string decodedStr;
    for (auto byte : decoded) {
        decodedStr += (char)byte;
    }
    eventCode = decodedStr;
}

void Event::execute() {
    duk::Context ctx = initContext();
    ctx.evalStringNoRes(eventCode.c_str());
}

duk::Context Event::initContext() {
    duk::Context context;

    duk_push_c_function(context, native_print, 1);
    duk_put_global_string(context, "print");

    context.registerClass<GameContext>();
    context.registerClass<GameSprite>();
    return context;
}

void Event::eval(std::string str) {
    duk::Context context = initContext();
    try {
        context.evalStringNoRes(str.c_str());
    } catch (duk::DuktapeException &ex) {
        std::cerr << ex.what() << std::endl;
    }
}
