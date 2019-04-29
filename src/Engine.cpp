#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>

#include <Engine.h>
#include <Log.h>
#include <module/Audio.h>
#include <module/Keyboard.h>
#include <Util.h>
#include <Main.h>
#include <Sprite.h>
#include <MapLoader.h>
#include <module/Registries.h>
#include <loader/DataLoader.h>
#include <types/Bitmap.h>

Object *Engine::player;
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
bool Engine::f_state = false;
bool Engine::f_flag = false;

int Engine::gui_flags = 0;

int Engine::load_state = 0;

std::vector<float> Engine::progressBars = {0, 0};

Gui *Engine::currentGui = nullptr;

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
    load_state |= STATE_DISPLAY;
    // Set icon and title
    al_set_display_icon(display, Engine::loadImage("sys:icon"));
    al_set_window_title(display, "FCW the RPG");

    // Set 60 FPS Timer
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        Log::error("Error initializing Timer");
        return false;
    }
    load_state |=  STATE_TIMER;
    al_start_timer(timer);

    // Create event queue
    eventQueue = al_create_event_queue();
    if (!eventQueue) {
        Log::error("Error initializing Event Queue");
        return false;
    }
    load_state |= STATE_EVENT_QUEUE;
    // Register events
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_display_event_source(display));

    redraw = false;
    done = false;
    oldTime = al_get_time();
    delayTime = al_get_time();

    updateLoadingProgress(0, 0.1);

    if (!DataLoader::load()) {
        Log::error("Error loading data");
        return false;
    }

    if (!MapLoader::loadMaps()) {
        Log::error("Error loading maps");
        return false;
    }

    load_state |= STATE_LOADED;

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
        if (event.type == ALLEGRO_EVENT_KEY_DOWN ||
                event.type == ALLEGRO_EVENT_KEY_UP ||
                event.type == ALLEGRO_EVENT_KEY_CHAR) {
            Keyboard::registerKeyEvent(event);
        }

        // If there is a map loaded, hand the event over
        if (current_map) {
            current_map->handleEvent(event);
        }
    }
    // If the event queue is empty, and the screen needs to be refreshed, redraw the screen
    if (redraw && al_is_event_queue_empty(eventQueue)) {
        // Clear redraw flag
        redraw = false;

        // Toggle Fullscreen on ALT+ENTER
        if (Keyboard::getKeyState(ALLEGRO_KEY_ALT) && Keyboard::getKeyState(ALLEGRO_KEY_ENTER)) {
            if (!f_flag) {
                f_state = !f_state;
                setFullscreen(f_state);
                f_flag = true;
            }
        } else {
            if (f_flag) {
                f_flag = false;
            }
        }

        handleControls();

        // Create screen buffer
        ALLEGRO_BITMAP *buffer = al_create_bitmap(SCREEN_W, SCREEN_H);
        // Set as draw target
        al_set_target_bitmap(buffer);
        // Clear to black
        al_clear_to_color(al_map_rgb(0x00, 0x00, 0x00));

        // Draw map and related to buffer
        if (current_map) {
            player->draw();
            current_map->draw();
            if (player) {
                // Update viewport
                current_map->updateViewport(player, false);
            }
        }

        if (currentGui) {
            currentGui->draw();
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

void Engine::handleControls() {
    if (player == nullptr) {
        return;
    }

    float playerX = player->getX();
    float playerY = player->getY();

    if (Keyboard::getKeyState(ALLEGRO_KEY_UP) /*|| Keyboard::getKeyState(ALLEGRO_KEY_W)*/) {
        if (currentGui) {
            if (!(gui_flags & GUI_UP)) {
                currentGui->handleKey(GUI_UP);
                gui_flags |= GUI_UP;
            }
        } else {
            playerY -= 4;
        }
    }else {
         gui_flags &= ~GUI_UP;

    }
    if (Keyboard::getKeyState(ALLEGRO_KEY_DOWN) /*|| Keyboard::getKeyState(ALLEGRO_KEY_S)*/) {
        if (currentGui) {
            if (!(gui_flags & GUI_DOWN)) {
                currentGui->handleKey(GUI_DOWN);
                gui_flags |= GUI_DOWN;
            }
        } else {
            playerY += 4;
        }
    } else {
        gui_flags &= ~GUI_DOWN;
    }

    if (Keyboard::getKeyState(ALLEGRO_KEY_LEFT) /*|| Keyboard::getKeyState(ALLEGRO_KEY_A)*/) {
        if (currentGui) {
            if (!(gui_flags & GUI_LEFT)) {
                currentGui->handleKey(GUI_LEFT);
                gui_flags |= GUI_LEFT;
            }
        } else {
            playerX -= 4;
        }
    } else {
        gui_flags &= ~GUI_LEFT;
    }

    if (Keyboard::getKeyState(ALLEGRO_KEY_RIGHT) /*|| Keyboard::getKeyState(ALLEGRO_KEY_D)*/) {
        if (currentGui) {
            if (!(gui_flags & GUI_RIGHT)) {
                currentGui->handleKey(GUI_RIGHT);
                gui_flags |= GUI_RIGHT;
            }
        } else {
            playerX += 4;
        }
    } else {
        gui_flags &= ~GUI_RIGHT;
    }

    if (Keyboard::getKeyState(ALLEGRO_KEY_ENTER) /*|| Keyboard::getKeyState(ALLEGRO_KEY_Z)*/) {
        if (currentGui) {
            if (!(gui_flags & GUI_SELECT)) {
                currentGui->handleKey(GUI_SELECT);
                gui_flags |= GUI_SELECT;
            }
        }
    } else {
        gui_flags &= ~GUI_SELECT;
    }

    player->setPosition(playerX, playerY);

    Object *collision;
    if (current_map && (collision = current_map->checkCollision(player)) != nullptr) {
        std::vector<float> fix = BoundingBox::fixCollision(collision->boundingBox, player->boundingBox);
        player->setPosition(fix[0], fix[1]);
    }
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
    }
}

void Engine::run() {
    // Get current system time
    long long int start = Util::getMilliTime();
    Log::info("Initializing...");

    // Run initialization
    if (!Engine::init()) {
        // If failed, quit program
        Log::error("Error in init");
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

    while (!done) {
        update();
    }

    Exit(0);
}

void Engine::Exit(int code) {
    if (load_state & STATE_LOADED) {
        for (auto s : Registries::spriteRegistry) {
            s.second->unload();
        }

        for (auto g : Registries::guiRegistry) {
            g.second->unload();
        }

        for (auto b : Bitmap::bitmaps) {
            b->destroy();
        }
    }

    if (load_state & STATE_EVENT_QUEUE) {
        al_destroy_event_queue(eventQueue);
    }
    if (load_state & STATE_TIMER) {
        al_destroy_timer(timer);
    }
    if (load_state & STATE_DISPLAY) {
        al_destroy_display(display);
    }

    exit(code);
}

void Engine::loadFonts() {
    Log::debug("Loading fonts");
    fonts["default"] = Engine::loadFont("sys:font_8bit", 16);

    // Load in all the fonts and make them available as a map
    fonts["font8"]  = Engine::loadFont("sys:font_8bit",  8);
    fonts["font16"] = Engine::loadFont("sys:font_8bit", 16);
    fonts["font24"] = Engine::loadFont("sys:font_8bit", 24);
    fonts["font32"] = Engine::loadFont("sys:font_8bit", 32);
}

ALLEGRO_BITMAP *Engine::loadImage(const char *file) {
    ALLEGRO_BITMAP *img = nullptr;
    ResourceFile *res = Registries::resourceFileRegistry.get(file);
    img = al_load_bitmap_f(res->openAllegroFile(), ".png");
    if (img != nullptr) {
        return img;
    }
    throw FileException("Error loading image file \"" + std::string(file) + "\"");
}

ALLEGRO_SAMPLE *Engine::loadSample(const char *file) {
    ALLEGRO_SAMPLE *sample = nullptr;
    ResourceFile *res = Registries::resourceFileRegistry.get(file);
    if ((sample = al_load_sample_f(res->openAllegroFile(), ".ogg")) != nullptr) {
        return sample;
    }
    throw FileException("Error loading sound file \"" + std::string(file) + "\"");
}

ALLEGRO_FONT *Engine::loadFont(const char *file, int size) {
    ALLEGRO_FONT *font = nullptr;
    ResourceFile *res = Registries::resourceFileRegistry.get(file);
    if ((font = al_load_ttf_font_f(res->openAllegroFile(), "", size, 0)) != nullptr) {
        return font;
    }
    throw FileException("Error loading font file \"" + std::string(file) + "\"");
}

void Engine::openGui(std::string gui) {
    currentGui = Registries::guiRegistry.get(gui);
}

void Engine::closeGui() {
    currentGui = nullptr;
}

void Engine::updateLoadingProgress(int which, float inc) {
    progressBars[which] += inc;
    refreshProgress();
}

void Engine::setLoadingProgress(int which, float amt) {
    progressBars[which] = amt;
    refreshProgress();
}

void Engine::refreshProgress() {
    /*int min_x = 16;
    int max_x = SCREEN_W - 16;
    int min_y = SCREEN_H - 32;
    int max_y = SCREEN_H - 16;
    int border = 2;
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR yellow = al_map_rgb(255, 255, 0);


    for (int x = min_x; x < max_x; x++) {
        for (int y = min_y; y < max_y; y++) {
            if (x < min_x+border || x > max_x-border-1 || y < min_y+border || y > max_y-border-1) {
                al_draw_pixel(x, y, white);
            }
        }
    }

    for (int x = min_x+border; x < max_x-border; x++) {
        if (x < min_x + (int) amt) {
            for (int y = min_y+border; y < max_y-border; y++) {
                al_draw_pixel(x, y, yellow);
            }
        }
    }*/

    al_clear_to_color(al_map_rgb(0, 0, 0));

    updateProgress(2, progressBars[0]);
    updateProgress(1, progressBars[1]);
    //updateProgress(0, progressBars[2]);

    al_flip_display();
}

void Engine::updateProgress(int pos, float amount) {
    int min_x = 16;
    int max_x = SCREEN_W - 16;
    int min_y = SCREEN_H - (32 + (32 * pos));
    int max_y = SCREEN_H - (16 + (32 * pos));

    int width = max_x-min_x;

    int border = 2;
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR yellow = al_map_rgb(255, 255, 0);

    for (int x = min_x; x < max_x; x++) {
        for (int y = min_y; y < max_y; y++) {
            if (x < min_x+border || x > max_x-border-1 || y < min_y+border || y > max_y-border-1) {
                al_draw_pixel(x, y, white);
            }
        }
    }

    for (int x = min_x+border; x < max_x-border; x++) {
        if (x < min_x + (int) (amount * width)) {
            for (int y = min_y+border; y < max_y-border; y++) {
                al_draw_pixel(x, y, yellow);
            }
        }
    }
}
