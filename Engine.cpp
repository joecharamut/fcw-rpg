

#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <thread>
#include "Engine.h"
#include "Log.h"
#include "Music.h"
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

// Function to initialize the game engine
int Engine::init() {
    // Initialize Allegro
    if(al_init() == 0) {
        Log::error("Error initializing Allegro");
        return 0;
    }
    // And all of the addons needed
    if(!al_init_font_addon()) {
        Log::error("Error initializing Allegro Font");
        return 0;
    }
    if(!al_init_ttf_addon()) {
        Log::error("Error initializing Allegro TTF Addon");
        return 0;
    }
    if(!al_init_image_addon()) {
        Log::error("Error initializing Allegro Image");
        return 0;
    }
    if(!al_install_keyboard()) {
        Log::error("Error initializing Keyboard");
        return 0;
    }
    if(!al_install_audio()) {
        Log::error("Error initializing Audio");
        return 0;
    }
    if(!al_init_acodec_addon()) {
        Log::error("Error initializing Audio Codec");
        return 0;
    }

    // Initialize my modules
    if (!Music::init()) {
        Log::error("Error initializing Audio Module");
        return 0;
    }
    if (!Keyboard::init()) {
        Log::error("Error initializing Keyboard Module");
        return 0;
    }

    // Create the window
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        Log::error("Error creating display");
        return 0;
    }
    al_set_new_display_flags(ALLEGRO_WINDOWED);
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

    return 1;
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
        if (current_map) {
            current_map->draw();
            // TODO: Remove this
            //current_map->updateViewport(hat, false);
        }

        // Calculate fps
        newTime = al_get_time();
        if ((newTime-delayTime) > 0.15) {
            fps = (1 / (newTime - oldTime));
            delayTime = newTime;
        }
        // Reset newTime
        oldTime = newTime;
        // And display it
        al_draw_textf(fonts["font16"], al_map_rgb(0xff, 0xff, 0xff), 0, 0, 0, "%.1f fps", (float) fps);

        // Flip the buffer to the display
        al_flip_display();
    }

    // Run music update routine - Used to fade audio
    Music::update();
}

void Engine::renderThread() {
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

template <typename T>
bool vectorContains(std::vector<T> vector, T object) {
    for (const auto &item : vector) {
        if (item == object) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> validCommands = {"help"};
std::map<std::string, std::string> commandHelp = {
        {"help", "Display help text and list commands."}
};

void processCommandString(std::string command) {
    auto commandPart = Util::splitString(command, " ");
    auto cmd = commandPart[0];

    if (!vectorContains(validCommands, cmd)) {
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
            if (!vectorContains(validCommands, commandPart[1])) {
                printf("\"%s\" is not a valid command.\n\n", commandPart[1].c_str());
            } else {
                printf("%s: %s\n\n", commandPart[1].c_str(), commandHelp[commandPart[1]].c_str());
            }
        }
    }
}

void Engine::commandThread() {
    while (!done) {
        std::string input;
        std::getline(std::cin, input);
        if (!input.empty()) {
            processCommandString(input);
        }
    }
}

void Engine::eventThread() {
    while (!done) {
        if (current_map) {
            for (auto event : current_map->current_room->events) {

            }
        }
    }
}

void Engine::run() {
    std::thread renderThread(Engine::renderThread);
    std::thread commandThread(Engine::commandThread);
    std::thread eventThread(Engine::eventThread);

    commandThread.detach();
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


