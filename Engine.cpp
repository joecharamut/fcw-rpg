#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <thread>
#include "Engine.h"
#include "Log.h"
#include "Audio.h"
#include "Keyboard.h"
#include "Util.h"
#include "Main.h"
#include "Sprite.h"
#include "MapLoader.h"

Sprite *Engine::player;
Map *Engine::current_map;
std::map<std::string, ALLEGRO_FONT *> Engine::fonts;
ALLEGRO_DISPLAY *Engine::display;
ALLEGRO_TIMER *Engine::timer;
ALLEGRO_EVENT_QUEUE *Engine::eventQueue;
bool Engine::redraw;
std::atomic<bool> Engine::done;
double Engine::oldTime;
double Engine::newTime;
double Engine::delayTime;
double Engine::fps;

int Engine::f_scale_w;
int Engine::f_scale_h;
int Engine::f_pos_w;
int Engine::f_pos_h;

int Engine::state = 0;

Registry<ResourceFile *> Engine::resourceFileRegistry;
Registry<Map *> Engine::mapRegistry;
Registry<Room *> Engine::roomRegistry;

// Function to initialize the game engine
bool Engine::init() {
    // Initialize Allegro
    Log::debug("Initializing Allegro");
    if(al_init() == 0) {
        Log::error("Error initializing Allegro");
        return false;
    }
    state |= STATE_ALLEGRO_INIT;

    // And all of the addons needed
    Log::debug("Initializing Font");
    if(!al_init_font_addon()) {
        Log::error("Error initializing Allegro Font");
        return false;
    }
    state |= STATE_ALLEGRO_FONT;

    Log::debug("Initializing TTF");
    if(!al_init_ttf_addon()) {
        Log::error("Error initializing Allegro TTF");
        return false;
    }
    state |= STATE_ALLEGRO_TTF;

    Log::debug("Initializing Image");
    if(!al_init_image_addon()) {
        Log::error("Error initializing Allegro Image");
        return false;
    }
    state |= STATE_ALLEGRO_IMAGE;

    Log::debug("Initializing Keyboard");
    if(!al_install_keyboard()) {
        Log::error("Error initializing Keyboard");
        return false;
    }
    state |= STATE_ALLEGRO_KEYBOARD;

    Log::debug("Initializing Audio");
    if(!al_install_audio()) {
        Log::error("Error initializing Audio");
        return false;
    }
    state |= STATE_ALLEGRO_AUDIO;

    Log::debug("Initializing Audio Codec");
    if(!al_init_acodec_addon()) {
        Log::error("Error initializing Audio Codec");
        return false;
    }

    // Initialize my modules
    Log::debug("Initializing Audio Module");
    if (!Audio::init()) {
        Log::error("Error initializing Audio Module");
        return false;
    }
    Log::debug("Initializing Keyboard Module");
    if (!Keyboard::init()) {
        Log::error("Error initializing Keyboard Module");
        return false;
    }

    // Create the window
    Log::debug("Creating Window");
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        Log::error("Error creating display");
        return false;
    }
    state |= STATE_DISPLAY;
    // Set icon and title
    al_set_display_icon(display, Engine::loadImage("sys:icon"));
    al_set_window_title(display, "FCW the RPG");

    // Set 60 FPS Timer
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        Log::error("Error initializing Timer");
        return false;
    }
    state |=  STATE_TIMER;
    al_start_timer(timer);

    // Create event queue
    eventQueue = al_create_event_queue();
    if (!eventQueue) {
        Log::error("Error initializing Event Queue");
        return false;
    }
    state |= STATE_EVENT_QUEUE;
    // Register events
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_display_event_source(display));

    redraw = false;
    done = false;
    oldTime = al_get_time();
    delayTime = al_get_time();

    if (!MapLoader::loadMaps()) {
        Log::error("Error loading maps");
        return false;
    }

    return true;
}

// Main game loop
void Engine::update() {
    // If there are more events, process one of them
    ALLEGRO_EVENT event;
    if (al_get_next_event(eventQueue, &event)) {
        // If the fps timer ticks, set screen redraw
        if (event.type == ALLEGRO_EVENT_TIMER) redraw = true;
        // If user clicks close button, end the game
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) done = true;
        // If user presses [ESC], close game
        // TODO: Replace with menu
        if (event.type == ALLEGRO_EVENT_KEY_UP && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) done = true;
        // Register a key event with the keyboard module
        if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP)
            Keyboard::registerKeyEvent(event);

        // If there is a map loaded, hand the event over
        if (current_map) {
            current_map->handleEvent(event);
        }
    }
    // If the event queue is empty, and the screen needs to be refreshed, redraw the screen
    if (redraw && al_is_event_queue_empty(eventQueue)) {
        // Clear redraw flag
        redraw = false;

        // Create screen buffer
        ALLEGRO_BITMAP *buffer = al_create_bitmap(SCREEN_W, SCREEN_H);
        // Set as draw target
        al_set_target_bitmap(buffer);
        // Clear to black
        al_clear_to_color(al_map_rgb(0x00, 0x00, 0x00));

        // Draw map and related to buffer
        if (current_map) {
            current_map->draw();
            if (player) {
                // Update viewport
                current_map->updateViewport(player, false);
            }
        }

        // Reset target to display
        al_set_target_bitmap(al_get_backbuffer(display));

        // Clear screen to black
        al_clear_to_color(al_map_rgb(0x00, 0x00, 0x00));

        // Check if we want fullscreen mode
        if (al_get_display_flags(display) & ALLEGRO_FULLSCREEN_WINDOW) {
            // Scale and draw the buffer
            al_draw_scaled_bitmap(buffer, 0, 0, SCREEN_W, SCREEN_H, f_pos_w, f_pos_h, f_scale_w, f_scale_h, 0);
        } else {
            // Just draw it
            al_draw_bitmap(buffer, 0, 0, 0);
        }

        // Delete buffer
        al_destroy_bitmap(buffer);

        // Calculate fps
        newTime = al_get_time();
        if ((newTime-delayTime) > 0.15) {
            fps = (1 / (newTime - oldTime));
            delayTime = newTime;
        }
        // Reset newTime
        oldTime = newTime;
        // And display it
        al_draw_textf(fonts["font16"], al_map_rgb(0xff, 0xff, 0xff), 0, 0, 0, "%.0f fps", (float) fps);

        // Flip the display buffer to the display
        al_flip_display();
    }

    // Run music update routine - Used to fade audio
    Audio::update();
}

void Engine::setFullscreen(bool enable) {
    if (enable) {
        al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);

        int window_w = al_get_display_width(display);
        int window_h = al_get_display_height(display);
        float scale = ((float)window_h/(float)SCREEN_H);

        f_scale_w = (int)(SCREEN_W*scale);
        f_scale_h = (int)(SCREEN_H*scale);
        f_pos_w = ((window_w/2) - (f_scale_w/2));
        f_pos_h = ((window_h/2) - (f_scale_h/2));
    } else {
        al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, false);
    }
}

std::vector<std::string> validCommands = {
        "help",
        "eval",
        "test"
};

std::map<std::string, std::string> commandHelp = {
        {"help", "Display help text and list commands."},
        {"eval", "Eval an event string."}
};

void processCommandString(std::string command) {
    auto commandPart = Util::splitString(command, " ");
    auto cmd = commandPart[0];

    //Log::debug("len: " + std::to_string(commandPart.size()));

    if (!Util::vectorContains(validCommands, cmd)) {
        printf("Invalid command \"%s\"\n", cmd.c_str());
        return;
    }

    if (cmd == "help") {
        if (commandPart.size() == 1) {
            printf("Valid Commands: \n");
            for (auto c : validCommands) {
                printf("  %s\n", c.c_str());
            }
            printf("Type help <command> for additional command help.\n\n");
        } else {
            if (!Util::vectorContains(validCommands, commandPart[1])) {
                printf("\"%s\" is not a valid command.\n\n", commandPart[1].c_str());
            } else {
                printf("%s: %s\n\n", commandPart[1].c_str(), commandHelp[commandPart[1]].c_str());
            }
        }
    } else if (cmd == "eval") {
        if (commandPart.size() < 2) {
            printf("Usage: eval <string>\n\n");
        } else {
            std::string evalString;
            for (int i = 1; i < commandPart.size(); i++) {
                evalString += commandPart[i] + " ";
            }
            Event::eval(evalString);
        }
    } else if (cmd == "test") {
        std::string testStr =
                "var ctx = new GameContext();\n"
                "var player = ctx.player;\n"
                "print(player.id);\n";
        Event::eval(testStr);
    }
}

void Engine::renderThreadFunction() {
    while (!done) {
        update();
    }
}

void Engine::commandThreadFunction() {
    while (!done) {
        std::string input;
        std::getline(std::cin, input);
        if (!input.empty()) {
            processCommandString(input);
        }
    }
}

void Engine::eventThreadFunction() {
    while (!done) {
        if (current_map) {
            for (auto event : current_map->current_room->events) {
                //event->execute();
            }
        }
    }
}

std::thread Engine::renderThread;
std::thread Engine::consoleThread;
std::thread Engine::eventThread;

void Engine::run() {
    // Get current system time
    long long int start = Util::getMilliTime();
    Log::info("Initializing...");

    // Run initialization
    if (!Engine::init()) {
        // If failed, quit program
        Log::error("Error in init!");
        Engine::Exit(1);
    }

    // Get system time
    long long int end = Util::getMilliTime();
    // Print how long it took
    Log::info("Initialization completed in " + std::to_string(end-start) + " ms");

    // Load Fonts
    loadFonts();

    // Testing code TODO: Remove
    Main::testing();

    renderThread = std::thread(Engine::renderThreadFunction);
    consoleThread = std::thread(Engine::commandThreadFunction);
    //eventThread = std::thread(Engine::eventThreadFunction);

    consoleThread.detach();
    //eventThread.detach();
    renderThread.join();
}

void Engine::Exit(int code) {
    if (state & STATE_EVENT_QUEUE) {
        al_destroy_event_queue(eventQueue);
    }
    if (state & STATE_TIMER) {
        al_destroy_timer(timer);
    }
    if (state & STATE_DISPLAY) {
        al_destroy_display(display);
    }
    if (state & STATE_ALLEGRO_AUDIO) {
        al_uninstall_audio();
    }
    if (state & STATE_ALLEGRO_KEYBOARD) {
        al_uninstall_keyboard();
    }
    if (state & STATE_ALLEGRO_IMAGE) {
        al_shutdown_image_addon();
    }
    if (state & STATE_ALLEGRO_TTF) {
        al_shutdown_ttf_addon();
    }
    if (state & STATE_ALLEGRO_FONT) {
        al_shutdown_font_addon();
    }
    if (state & STATE_ALLEGRO_INIT) {
        al_uninstall_system();
    }

    exit(code);
}

void Engine::loadFonts() {
    Log::debug("Loading fonts");
    // Load in all the fonts and make them available as a map
    fonts["font8"]  = Engine::loadFont("sys:font_8bit",  8);
    fonts["font16"] = Engine::loadFont("sys:font_8bit", 16);
    fonts["font24"] = Engine::loadFont("sys:font_8bit", 24);
    fonts["font32"] = Engine::loadFont("sys:font_8bit", 32);
}

ALLEGRO_BITMAP *Engine::loadImage(const char *file) {
    ALLEGRO_BITMAP *img = nullptr;
    ResourceFile *res = resourceFileRegistry.get(file);
    img = al_load_bitmap_f(res->openAllegroFile(), ".png");
    if (img != nullptr) {
        return img;
    }
    throw FileException("Error loading image file \"" + std::string(file) + "\"");
}

ALLEGRO_SAMPLE *Engine::loadSample(const char *file) {
    ALLEGRO_SAMPLE *sample = nullptr;
    ResourceFile *res = resourceFileRegistry.get(file);
    if ((sample = al_load_sample_f(res->openAllegroFile(), ".ogg")) != nullptr) {
        return sample;
    }
    throw FileException("Error loading sound file \"" + std::string(file) + "\"");
}

ALLEGRO_FONT *Engine::loadFont(const char *file, int size) {
    ALLEGRO_FONT *font = nullptr;
    ResourceFile *res = resourceFileRegistry.get(file);
    if ((font = al_load_ttf_font_f(res->openAllegroFile(), "", size, 0)) != nullptr) {
        return font;
    }
    throw FileException("Error loading font file \"" + std::string(file) + "\"");
}


