#include "Event.h"
#include "duktape.h"
#include "Main.h"
#include "Engine.h"
#include <cstdio>

/*// Duktape Functions
static duk_ret_t native_print(duk_context *ctx) {
    printf("%s\n", duk_to_string(ctx, 0));
    return 0;
}

static duk_ret_t getGameContext(duk_context *ctx) {
    duk_idx_t gameContext = duk_push_object(ctx);

    duk_idx_t musicArray = duk_push_array(ctx);
    int i = 0;
    for (const auto &m : Engine::current_map->music) {
        duk_push_string(ctx, m.first.c_str());
        duk_put_prop_index(ctx, musicArray, i++);
    }
    duk_put_prop_string(ctx, musicArray, "music");

    return 1;
}

void Event::test() {
    duk_context *ctx = duk_create_heap_default();

    // Register Functions
    duk_push_c_function(ctx, native_print, 1);
    duk_put_global_string(ctx, "print");

    duk_push_c_function(ctx, getGameContext, 0);
    duk_put_global_string(ctx, "getGameContext");

    std::string testString = "dmFyIG11c2ljID0gZ2V0R2FtZUNvbnRleHQoKS5tdXNpYzsKZm9yICh2YXIgaSA9IDA7IGkgPCBtdXNpYy5sZW5ndGg7IGkrKykgewogICAgcHJpbnQobXVzaWNbaV0pOwp9";

    duk_push_string(ctx, testString.c_str());
    duk_base64_decode(ctx, -1);
    duk_buffer_to_string(ctx, -1);
    duk_eval_string_noresult(ctx, duk_to_string(ctx, -1));

    duk_destroy_heap(ctx);
}*/

#include <duktape-cpp/DuktapeCpp.h>

static duk_ret_t native_print(duk_context *ctx) {
    printf("%s\n", duk_to_string(ctx, 0));
    return 0;
}

class GameContext {
public:
    GameContext() = default;

    std::string currentMap() const { return "testing"; }

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<GameContext>);
        i.property("current_map", &GameContext::currentMap);
    }
};
DUK_CPP_DEF_CLASS_NAME(GameContext);

void Event::test() {
    duk::Context ctx;

    duk_push_c_function(ctx, native_print, 1);
    duk_put_global_string(ctx, "print");

    ctx.registerClass<GameContext>();
    ctx.evalStringNoRes("print(new GameContext().current_map);");
}
