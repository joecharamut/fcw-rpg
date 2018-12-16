#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <chrono>

#include "Sprite.h"
#include "Map.h"
#include "Util.h"
#include "Music.h"
#include "Object.h"
#include "Main.h"
#include "Keyboard.h"

bool redraw = true;

Map *current_map = nullptr;

ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER *timer;
ALLEGRO_DISPLAY *display;
double oldTime, newTime, delayTime, fps = 0;

bool Main::done = false;
std::map<std::string, ALLEGRO_FONT *> Main::fonts;

Sprite *hat;

// Main game loop
void Main::update() {
    // If there are more events, process one of them
    ALLEGRO_EVENT event;
    if (al_get_next_event(queue, &event)) {
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
    if(redraw && al_is_event_queue_empty(queue)) {
        // Clear redraw flag
        redraw = false;
        // Clear screen to black
        al_clear_to_color(al_map_rgb(0x00, 0x00, 0x00));
        // If a map is loaded, let it draw its stuff
        if (current_map) {
            current_map->draw();
            // TODO: Remove this
            current_map->updateViewport(hat, false);
        }

        // Process Global Events, e.g. movement, menu
        Main::executeGlobalEvents();

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

std::vector<Event *> Main::globalEvents;

bool Main::initGlobalEvents() {
    //globalEvents.push_back(Event::decode("{SAMPLE}//{EVENT}"));
    return true;
}

void Main::executeGlobalEvents() {
    for (auto event : globalEvents) {
        if (current_map)
            event->doEvent(current_map);
    }
}

void mapEventHandler(ALLEGRO_EVENT event) {
    if (event.type == ALLEGRO_EVENT_TIMER) {
        Sprite *hat = current_map->getSpriteById("s_hat");
        if (hat != nullptr) {
            float hat_x = hat->x;
            float hat_y = hat->y;
            if (Keyboard::getKeyState(ALLEGRO_KEY_UP) || Keyboard::getKeyState(ALLEGRO_KEY_W)) {
                hat_y -= 4;
            }
            if (Keyboard::getKeyState(ALLEGRO_KEY_DOWN) || Keyboard::getKeyState(ALLEGRO_KEY_S)) {
                hat_y += 4;
            }
            if (Keyboard::getKeyState(ALLEGRO_KEY_LEFT) || Keyboard::getKeyState(ALLEGRO_KEY_A)) {
                hat_x -= 4;
            }
            if (Keyboard::getKeyState(ALLEGRO_KEY_RIGHT) || Keyboard::getKeyState(ALLEGRO_KEY_D)) {
                hat_x += 4;
            }

            hat->setX(hat_x);
            hat->setY(hat_y);

            Sprite *spr = current_map->checkCollision(hat);
            if (spr) {
                std::vector<float> fix = BoundingBox::fixCollision(spr->boundingBox, hat->boundingBox);
                hat->setX(fix[0]);
                hat->setY(fix[1]);
            }
        }
    }
}

void Main::loadFonts() {
    // Load in all the fonts and make them available as a map
    fonts["font8"]  = al_load_ttf_font("resources/font/DOSVGA.ttf",  8, 0);
    fonts["font16"] = al_load_ttf_font("resources/font/DOSVGA.ttf", 16, 0);
    fonts["font24"] = al_load_ttf_font("resources/font/DOSVGA.ttf", 24, 0);
    fonts["font32"] = al_load_ttf_font("resources/font/DOSVGA.ttf", 32, 0);
}

// Function to initialize the game engine
int Main::init() {
    // Initialize Allegro
    if(al_init() == 0) {
        Util::log("Error initializing Allegro", "INIT", ERROR);
        return 0;
    }
    // And all of the addons needed
    if(!al_init_font_addon()) {
        Util::log("Error initializing Allegro Font", "INIT", ERROR);
        return 0;
    }
    if(!al_init_ttf_addon()) {
        Util::log("Error initializing Allegro TTF", "INIT", ERROR);
        return 0;
    }
    if(!al_init_image_addon()) {
        Util::log("Error initializing Allegro Image", "INIT", ERROR);
        return 0;
    }
    if(!al_install_keyboard()) {
        Util::log("Error initializing Keyboard", "INIT", ERROR);
        return 0;
    }
    if(!al_install_audio()) {
        Util::log("Error initializing Audio", "INIT", ERROR);
        return 0;
    }
    if(!al_init_acodec_addon()) {
        Util::log("Error initializing Audio Codec", "INIT", ERROR);
        return 0;
    }

    // Setup Global Events
    initGlobalEvents();

    // Initialize my modules
    if (!Music::init()) {
        Util::log("Error initializing Audio Module", "INIT", ERROR);
        return 0;
    }
    if (!Keyboard::init()) {
        Util::log("Error initializing Keyboard Module", "INIT", ERROR);
        return 0;
    }

    // Create the window
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        Util::log("Error creating display", "INIT", ERROR);
        return 0;
    }
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    // Set icon and title
    al_set_display_icon(display, al_load_bitmap("resources/icon.png"));
    al_set_window_title(display, "FCW the RPG");

    // Set 60 FPS
    timer = al_create_timer(1.0 / FPS);
    al_start_timer(timer);
    queue = al_create_event_queue();
    // Register events
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_display_event_source(display));
    return 1;
}

// Function for testing features and stuff
void testing() {
    // Load the test map
    current_map = Map::loadMap("map_test");
    // Set the event handler TODO: Replace with events from map file, maybe pass events from game to map
    current_map->setEventHandlerFunction(mapEventHandler);

    // Load in some test music
    ALLEGRO_SAMPLE_INSTANCE *music = current_map->music.at("mus_cave");
    al_set_sample_instance_playmode(music, ALLEGRO_PLAYMODE_LOOP);
    Music::playMusic(music);

    // Set the hat position and click action
    hat = current_map->getSpriteById("s_hat");
    hat->setX(SCREEN_W/4.0f -(hat->width/2.0f));
    hat->setY(SCREEN_H/4.0f -(hat->height/2.0f));
}

int main(int argc, char *argv[]) {
    // Get current system time
    long long int start = Util::getMilliTime();
    Util::log("Initializing Engine");
    // Run initialization
    if (!Main::init()) {
        // If failed, quit program
        return 1;
    }
    // Get system time
    long long int end = Util::getMilliTime();
    // Print how long it took
    Util::log("Done (" + std::to_string(end-start) + " ms)");

    Util::log("Loading fonts");
    // Load fonts TODO: Move this
    Main::loadFonts();

    // TODO: Maybe load maps at start time
    Map::enumerateMaps();

    // Run some temp testing stuff
    // TODO: Eventually remove this
    testing();

    Util::log("Initialization Finished, Starting Game");
    // Set old time for fps counter
    oldTime = al_get_time();
    delayTime = al_get_time();
    // While we are not done, update the game
    while (!Main::done) {
        Main::update();
    }
}