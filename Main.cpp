#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

#include "Sprite.h"
#include "Textbox.h"
#include "Map.h"
#include "Globals.h"
#include "ActionSprite.h"
#include "Util.h"

const float FPS = 60;
const int SCREEN_H = 512;
const int SCREEN_W = 512;

enum KEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

bool redraw = true;
bool key[4] = { false, false, false, false };

Map *current_map = nullptr;

ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER *timer;
ALLEGRO_DISPLAY *display;

void update() {
    ALLEGRO_EVENT event;
    if (al_get_next_event(queue, &event)) {
        if (current_map) {
            current_map->handleEvent(event);
        }
    }
    if(redraw && al_is_event_queue_empty(queue)) {
        redraw = false;
        al_clear_to_color(al_map_rgb(0x00, 0x00, 0x00));
        if (current_map)
            current_map->draw();
        frameCounter = (++frameCounter % 60);
        al_flip_display();
    }
}

void clickFunction(ActionSprite *as, ALLEGRO_EVENT event) {
    printf("%i\n", current_map->checkCollision(as));
    switch (event.mouse.button) {
        case 1:
            printf("LMB\n");
            break;
        case 2:
            printf("RMB\n");
            break;
        default:
            printf("Other: %i\n", event.mouse.button);
            break;
    }
}

void mapEventHandler(ALLEGRO_EVENT event) {
    if (event.type == ALLEGRO_EVENT_TIMER) {
        Sprite *hat = current_map->getSpriteById("s_hat");
        if (hat != nullptr) {
            float hat_x = hat->x;
            float hat_y = hat->y;
            if(key[KEY_UP]) {
                hat_y -= 4;
            }
            if(key[KEY_DOWN]) {
                hat_y += 4;
            }
            if(key[KEY_LEFT]) {
                hat_x -= 4;
            }
            if(key[KEY_RIGHT]) {
                hat_x += 4;
            }

            hat->setX(hat_x);
            hat->setY(hat_y);
        }
        redraw = true;
    } else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        done = true;
    } else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        for(auto *spr : current_map->sprites) {
            auto *actionSprite = dynamic_cast<ActionSprite *>(spr);
            if (actionSprite) {
                if (BoundingBox::intersect(actionSprite->boundingBox, event.mouse.x, event.mouse.y)) {
                    clickFunction(actionSprite, event);
                }
            }
        }
    } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
        for(auto *spr : current_map->sprites) {
            auto *actionSprite = dynamic_cast<ActionSprite *>(spr);
            if (actionSprite) {
                if (BoundingBox::intersect(actionSprite->boundingBox, event.mouse.x, event.mouse.y)) {
                    if (actionSprite->hoverAction)
                        actionSprite->hoverAction(actionSprite, event);
                }
            }
        }
    } else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                key[KEY_UP] = true;
                break;

            case ALLEGRO_KEY_DOWN:
                key[KEY_DOWN] = true;
                break;

            case ALLEGRO_KEY_LEFT:
                key[KEY_LEFT] = true;
                break;

            case ALLEGRO_KEY_RIGHT:
                key[KEY_RIGHT] = true;
                break;

            case ALLEGRO_KEY_W:
                key[KEY_UP] = true;
                break;

            case ALLEGRO_KEY_S:
                key[KEY_DOWN] = true;
                break;

            case ALLEGRO_KEY_A:
                key[KEY_LEFT] = true;
                break;

            case ALLEGRO_KEY_D:
                key[KEY_RIGHT] = true;
                break;
            default:
                break;
        }
    } else if(event.type == ALLEGRO_EVENT_KEY_UP) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                key[KEY_UP] = false;
                break;

            case ALLEGRO_KEY_DOWN:
                key[KEY_DOWN] = false;
                break;

            case ALLEGRO_KEY_LEFT:
                key[KEY_LEFT] = false;
                break;

            case ALLEGRO_KEY_RIGHT:
                key[KEY_RIGHT] = false;
                break;

            case ALLEGRO_KEY_W:
                key[KEY_UP] = false;
                break;

            case ALLEGRO_KEY_S:
                key[KEY_DOWN] = false;
                break;

            case ALLEGRO_KEY_A:
                key[KEY_LEFT] = false;
                break;

            case ALLEGRO_KEY_D:
                key[KEY_RIGHT] = false;
                break;

            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            default:
                break;
        }
    }
}

void loadFonts() {
    font8  = al_load_ttf_font("resources/font/DOSVGA.ttf",  8, 0);
    font16 = al_load_ttf_font("resources/font/DOSVGA.ttf", 16, 0);
    font24 = al_load_ttf_font("resources/font/DOSVGA.ttf", 24, 0);
    font32 = al_load_ttf_font("resources/font/DOSVGA.ttf", 32, 0);
    fontMap["font8"] = font8;
    fontMap["font16"] = font16;
    fontMap["font24"] = font24;
    fontMap["font32"] = font32;
}

int main(int argc, char *argv[]) {
/*
    Util::log("Testing collision:");
    BoundingBox *b1 = new BoundingBox(32, 32, 128, 128);
    BoundingBox *b2 = new BoundingBox(64, 64, 72, 72);
    BoundingBox *b3 = new BoundingBox(16, 16, 24, 24);

    Util::log("Checking if point 64,64 intersects 32,32 x 128,128:", "Test");
    Util::log((std::string) "Intersection: " + (BoundingBox::intersect(b1, 64, 64) ? "Yes" : "No") + " (Expect: Yes)", "Test");

    Util::log("Checking if point 16,16 intersects 32,32 x 128,128:", "Test");
    Util::log((std::string) "Intersection: " + (BoundingBox::intersect(b1, 16, 16) ? "Yes" : "No") + " (Expect: No)", "Test");

    Util::log("Checking point remapping:", "Test");
    std::vector<float> test = BoundingBox::fixCollision(b1, 64, 64);
    Util::log((std::string) "Remap: 64,64 -> " + std::to_string(test[0]) + "," + std::to_string(test[1]) + " (Expect: 32,32)", "Test");

    Util::log("Checking point remapping:", "Test");
    test = BoundingBox::fixCollision(b1, 16, 16);
    Util::log((std::string) "Remap: 16,16 -> " + std::to_string(test[0]) + "," + std::to_string(test[1]) + " (Expect: 16,16)", "Test");

    Util::log("Checking if 64,64 x 72,72 intersects 32,32 x 128,128", "Test");
    Util::log((std::string) "Intersection: " + (BoundingBox::intersect(b1, b2) ? "Yes" : "No") + " (Expect: Yes)", "Test");

    Util::log("Checking if 16,16 x 24,24 intersects 32,32 x 128,128", "Test");
    Util::log((std::string) "Intersection: " + (BoundingBox::intersect(b1, b3) ? "Yes" : "No") + " (Expect: No)", "Test");

    Util::log("Checking box remapping:", "Test");
    test = BoundingBox::fixCollision(b1, b2);
    Util::log((std::string) "Remap: 64,64 x 72,72 -> " + std::to_string(test[0]) + "," + std::to_string(test[1]) +
              " x " + std::to_string(test[2]) + "," + std::to_string(test[3]) +
              " (Expect: 32,32 x 40,40)", "Test");

    Util::log("Checking box remapping:", "Test");
    test = BoundingBox::fixCollision(b1, b3);
    Util::log((std::string) "Remap: 16,16 x 24,24 -> " + std::to_string(test[0]) + "," + std::to_string(test[1]) +
              " x " + std::to_string(test[2]) + "," + std::to_string(test[3]) +
              " (Expect: 16,16 x 24,24)", "Test");

    Util::log("Test Complete");
*/
    done = false;
    Util::log("Initialising Game");

    if(al_init() == 0) {
        Util::log("Error initialising Allegro", "INIT", ERROR);
        return 1;
    }
    if(!al_init_font_addon()) {
        Util::log("Error initialising Allegro Font", "INIT", ERROR);
        return 1;
    }
    if(!al_init_ttf_addon()) {
        Util::log("Error initialising Allegro TTF", "INIT", ERROR);
        return 1;
    }
    if(!al_init_image_addon()) {
        Util::log("Error initialising Allegro Image", "INIT", ERROR);
        return 1;
    }
    if(!al_install_keyboard()) {
        Util::log("Error initialising Keyboard", "INIT", ERROR);
        return 1;
    }
    if(!al_install_mouse()) {
        Util::log("Error initialising Mouse", "INIT", ERROR);
        return 1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        Util::log("Error creating display", "INIT", ERROR);
        return 1;
    }
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    al_set_display_icon(display, al_load_bitmap("resources/icon.png"));
    al_set_window_title(display, "FCW the RPG");

    timer = al_create_timer(1.0 / FPS);
    al_start_timer(timer);
    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_display_event_source(display));

    Util::log("Loading Fonts");
    loadFonts();

    Util::log("Enumerating Maps");
    for (const auto &str : Map::enumerateMaps()) {
        Util::log(str);
    }
    Util::log("Done");

    //Map::test();
    //current_map = Map::loadMapFile("test.json");
    current_map = Map::loadMap("map_test");
    current_map->setEventHandlerFunction(mapEventHandler);

    Util::log("Initialisation Finished, Starting Game");
    while (!done) {
        update();
    }
}