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
        al_clear_to_color(al_map_rgb(0xff, 0xff, 0xff));
        if (current_map)
            current_map->draw();
        al_flip_display();
    }
}

void clickFunction(ActionSprite *as) {
    printf("Click\n");
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
        }
        redraw = true;
    } else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        done = true;
    } else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        LinkedSprite *next = current_map->sprites;
        while(next != nullptr) {
            if (next->sprite == nullptr) {
                next = next->next;
                continue;
            }
            ActionSprite *actionSprite = dynamic_cast<ActionSprite *>(next->sprite);
            if (actionSprite) {
                if (actionSprite->boundingBox->check(event.mouse.x, event.mouse.y)) {
                    if (actionSprite->clickAction)
                        actionSprite->clickAction(actionSprite);
                }
            }
            next = next->next;
        }
    } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
        LinkedSprite *next = current_map->sprites;
        while(next != nullptr) {
            if (next->sprite == nullptr) {
                next = next->next;
                continue;
            }
            ActionSprite *actionSprite = dynamic_cast<ActionSprite *>(next->sprite);
            if (actionSprite) {
                if (actionSprite->boundingBox->check(event.mouse.x, event.mouse.y)) {
                    if (actionSprite->hoverAction)
                        actionSprite->hoverAction(actionSprite);
                }
            }
            next = next->next;
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
    auto **tileset = new Tile*[4];
    tileset[0] =  new Tile(al_load_bitmap("resources/tile00.png"));
    tileset[1] =  new Tile(al_load_bitmap("resources/tile01.png"));
    tileset[2] =  new Tile(al_load_bitmap("resources/tile02.png"));
    tileset[3] =  new Tile(al_load_bitmap("resources/icon.png"));

    int **tilemap;
    tilemap = new int*[32];
    for (int i = 0; i < 32; i++){
        tilemap[i] = new int[32];
        for (int j = 0; j < 32; j++) {
            tilemap[i][j] = (j%4);
        }
    }
    current_map = new Map(0, const_cast<char*>("m_main_menu"), tileset, tilemap, 32, 32);
    current_map->setEventHandlerFunction(mapEventHandler);
    ALLEGRO_BITMAP *hatImage = al_load_bitmap("resources/hat.png");
    current_map->addSprite(new ActionSprite(0,0,hatImage,"s_hat", clickFunction, nullptr));
    current_map->addText("Hello I am some test text.", font24, al_map_rgb(0xff,0xff,0xff), 0, 0);
}

void loadFonts() {
    font8  = al_load_ttf_font("resources/font/DOSVGA.ttf",  8, 0);
    font16 = al_load_ttf_font("resources/font/DOSVGA.ttf", 16, 0);
    font24 = al_load_ttf_font("resources/font/DOSVGA.ttf", 24, 0);
    font32 = al_load_ttf_font("resources/font/DOSVGA.ttf", 32, 0);
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