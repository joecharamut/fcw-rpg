

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

Sprite *Engine::player;
Map *Engine::current_map;
std::map<std::string, ALLEGRO_FONT *> Engine::fonts;
ALLEGRO_DISPLAY *Engine::display;
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

// Function to initialize the game engine
bool Engine::init() {
    // Initialize Allegro
    Log::debug("Initializing Allegro");
    if(al_init() == 0) {
        Log::error("Error initializing Allegro");
        return false;
    }
    // And all of the addons needed
    Log::debug("Initializing Font");
    if(!al_init_font_addon()) {
        Log::error("Error initializing Allegro Font");
        return false;
    }
    Log::debug("Initializing TTF");
    if(!al_init_ttf_addon()) {
        Log::error("Error initializing Allegro TTF");
        return false;
    }
    Log::debug("Initializing Image");
    if(!al_init_image_addon()) {
        Log::error("Error initializing Allegro Image");
        return false;
    }
    Log::debug("Initializing Keyboard");
    if(!al_install_keyboard()) {
        Log::error("Error initializing Keyboard");
        return false;
    }
    Log::debug("Initializing Audio");
    if(!al_install_audio()) {
        Log::error("Error initializing Audio");
        return false;
    }
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
    // Set icon and title
    al_set_display_icon(display, Engine::loadImage("resources/icon.png"));
    al_set_window_title(display, "FCW the RPG");

    // Set 60 FPS Timer
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    al_start_timer(timer);

    // Create event queue
    eventQueue = al_create_event_queue();
    // Register events
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_display_event_source(display));

    redraw = false;
    done = false;
    oldTime = al_get_time();
    delayTime = al_get_time();

    return true;
}

// Main game loop
void Engine::update() {
    // If there are more events, process one of them
    ALLEGRO_EVENT event;
    if (al_get_next_event(eventQueue, &event)) {
        // If the 60fps timer ticks, set screen redraw
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
        // Clear screen to black
        al_clear_to_color(al_map_rgb(0x00, 0x00, 0x00));

        // If a map is loaded, let it draw its stuff
        ALLEGRO_BITMAP *screen = al_create_bitmap(SCREEN_W, SCREEN_H);
        al_set_target_bitmap(screen);
        al_clear_to_color(al_map_rgb(0x00, 0x00, 0x00));

        if (current_map) {
            current_map->draw();
            // TODO: Remove this maybe
            if (player) {
                current_map->updateViewport(player, false);
            }
        }

        al_set_target_bitmap(al_get_backbuffer(display));

        bool fullscreen = (bool)((al_get_display_flags(display) & ALLEGRO_FULLSCREEN_WINDOW) >> 9);
        if (fullscreen) {
            al_draw_scaled_bitmap(screen, 0, 0, SCREEN_W, SCREEN_H, f_pos_w, f_pos_h, f_scale_w, f_scale_h, 0);
        } else {
            al_draw_bitmap(screen, 0, 0, 0);
        }

        al_destroy_bitmap(screen);

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

        // Flip the buffer to the display
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

void Engine::renderThreadFunction() {
    // Get current system time
    long long int start = Util::getMilliTime();
    Log::info("Initializing Engine");

    // Run initialization
    if (!Engine::init()) {
        // If failed, quit program
        Log::error("Error in init!");
        return;
    }
    // Get system time
    long long int end = Util::getMilliTime();
    // Print how long it took
    Log::info("Done (" + std::to_string(end-start) + " ms)");

    // Load Fonts
    loadFonts();

    // Testing code TODO: Remove
    Main::testing();

    Log::info("Starting game loop");

    while (!done) {
        update();
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
            std::string evalString = "";
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

            }
        }
    }
}

std::thread Engine::renderThread;
std::thread Engine::consoleThread;
std::thread Engine::eventThread;

void Engine::run() {
    renderThread = std::thread(Engine::renderThreadFunction);
    consoleThread = std::thread(Engine::commandThreadFunction);
    eventThread = std::thread(Engine::eventThreadFunction);

    consoleThread.detach();
    eventThread.detach();
    renderThread.join();
}

void Engine::loadFonts() {
    Log::info("Loading fonts");
    // Load in all the fonts and make them available as a map
    fonts["font8"]  = Engine::loadFont("resources/font/DOSVGA.ttf",  8);
    fonts["font16"] = Engine::loadFont("resources/font/DOSVGA.ttf", 16);
    fonts["font24"] = Engine::loadFont("resources/font/DOSVGA.ttf", 24);
    fonts["font32"] = Engine::loadFont("resources/font/DOSVGA.ttf", 32);
}

ALLEGRO_BITMAP *Engine::loadImage(const char *file) {
    ALLEGRO_BITMAP *img = nullptr;
    if ((img = al_load_bitmap(file)) != nullptr) {
        return img;
    }
    throw FileException("Error loading image file \"" + std::string(file) + "\"");
}

ALLEGRO_SAMPLE *Engine::loadSample(const char *file) {
    ALLEGRO_SAMPLE *sample = nullptr;
    if ((sample = al_load_sample(file)) != nullptr) {
        return sample;
    }
    throw FileException("Error loading sound file \"" + std::string(file) + "\"");
}

ALLEGRO_FONT *Engine::loadFont(const char *file, int size) {
    ALLEGRO_FONT *font = nullptr;
    if ((font = al_load_ttf_font(file, size, 0)) != nullptr) {
        return font;
    }
    throw FileException("Error loading font file \"" + std::string(file) + "\"");
}


