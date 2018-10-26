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

const float FPS = 60;
const int SCREEN_H = 512;
const int SCREEN_W = 512;

enum KEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

bool redraw = true;
int hat_x = 0;
int hat_y = 0;
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
        Sprite *hat = current_map->getSpriteById("s_hat");
        if (hat != nullptr) {
            hat->setX(hat_x);
            hat->setY(hat_y);
            if (current_map->checkCollision(hat)) {
                hat->setX(--hat_x);
                hat->setY(--hat_y);
            }
        }
        redraw = true;
    } else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        done = true;
    } else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        for(auto *spr : current_map->sprites) {
            auto *actionSprite = dynamic_cast<ActionSprite *>(spr);
            if (actionSprite) {
                if (actionSprite->boundingBox->intersect(event.mouse.x, event.mouse.y)) {
                    if (actionSprite->clickAction)
                        actionSprite->clickAction(actionSprite, event);
                }
            }
        }
    } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
        for(auto *spr : current_map->sprites) {
            auto *actionSprite = dynamic_cast<ActionSprite *>(spr);
            if (actionSprite) {
                if (actionSprite->boundingBox->intersect(event.mouse.x, event.mouse.y)) {
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
        }
    }
}

void loadSprites() {
    //Map::test();
    //exit(0);
    current_map = Map::loadMap("test.json");
    current_map->setEventHandlerFunction(mapEventHandler);

    //current_map->addSprite(new ActionSprite(0,0,"resources/hat.png","s_hat", clickFunction, nullptr));
    //current_map->addText("Hello I am some test text.", "font24", 0xff,0xff,0xff, 0, 0);
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
    done = false;
    al_init();

    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();

    display = al_create_display(SCREEN_W, SCREEN_H);
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    al_set_display_icon(display, al_load_bitmap("resources/icon.png"));

    timer = al_create_timer(1.0 / FPS);
    al_start_timer(timer);

    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_display_event_source(display));


    al_set_window_title(display, "FCW the RPG");
    loadFonts();
    loadSprites();

    while (!done) {
        update();
    }
}