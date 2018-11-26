#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "Sprite.h"
#include "Textbox.h"
#include "Map.h"
#include "Globals.h"
#include "Util.h"
#include "Music.h"
#include "Event.h"

const float FPS = 60;

enum KEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

bool redraw = true;
bool key[4] = { false, false, false, false };

Map *current_map = nullptr;

ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER *timer;
ALLEGRO_DISPLAY *display;

ALLEGRO_SAMPLE_INSTANCE* music1;
ALLEGRO_SAMPLE_INSTANCE* music2;
int temp = 0;
double oldTime, newTime, fps = 0;

Sprite *hat;

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
        if (current_map) {
            current_map->draw();
            current_map->updateViewport(hat, false);
        }
        frameCounter = (++frameCounter % 60);

        newTime = al_get_time();
        if (frameCounter % 16 == 0)
            fps = 1 / (newTime - oldTime);
        oldTime = newTime;
        al_draw_textf(font24, al_map_rgb(0xff, 0xff, 0xff), 0, 32, 0, "FPS: %.1f", (float) fps);

        al_flip_display();
        Music::update();
    }
}

void clickFunction(Sprite *spr, ALLEGRO_EVENT event) {
    switch (event.mouse.button) {
        case 1:
            printf("X: %f, Y: %f\n", spr->x, spr->y);
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

            Sprite* spr = current_map->checkCollision(hat);
            if (spr) {
                std::vector<float> fix = BoundingBox::fixCollision(spr->boundingBox, hat->boundingBox);
                hat->setX(fix[0]);
                hat->setY(fix[1]);
            }
        }
        redraw = true;
    } else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        done = true;
    } else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        for(auto *spr : current_map->sprites) {
            if (spr->clickAction) {
                if (BoundingBox::intersect(spr->boundingBox, event.mouse.x, event.mouse.y)) {
                    spr->clickAction(spr, event);
                }
            }
        }
    } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
        for(auto *spr : current_map->sprites) {
            if (spr->hoverAction) {
                if (BoundingBox::intersect(spr->boundingBox, event.mouse.x, event.mouse.y)) {
                    spr->hoverAction(spr, event);
                }
            }
        }
    } else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
            case ALLEGRO_KEY_W:
                key[KEY_UP] = true;
                break;
            case ALLEGRO_KEY_DOWN:
            case ALLEGRO_KEY_S:
                key[KEY_DOWN] = true;
                break;
            case ALLEGRO_KEY_LEFT:
            case ALLEGRO_KEY_A:
                key[KEY_LEFT] = true;
                break;
            case ALLEGRO_KEY_RIGHT:
            case ALLEGRO_KEY_D:
                key[KEY_RIGHT] = true;
                break;
            case ALLEGRO_KEY_SPACE:
                if (temp) Music::playMusic(music1); else Music::playMusic(music2);
                temp = !temp;
                break;
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            default:
                break;
        }
    } else if(event.type == ALLEGRO_EVENT_KEY_UP) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
            case ALLEGRO_KEY_W:
                key[KEY_UP] = false;
                break;
            case ALLEGRO_KEY_DOWN:
            case ALLEGRO_KEY_S:
                key[KEY_DOWN] = false;
                break;
            case ALLEGRO_KEY_LEFT:
            case ALLEGRO_KEY_A:
                key[KEY_LEFT] = false;
                break;
            case ALLEGRO_KEY_RIGHT:
            case ALLEGRO_KEY_D:
                key[KEY_RIGHT] = false;
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
    done = false;

    Util::log("Initializing Engine");
    if((int)al_init() == 0) {
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
    if(!al_install_audio()) {
        Util::log("Error initialising Audio", "INIT", ERROR);
        return 1;
    }
    if(!al_init_acodec_addon()) {
        Util::log("Error initialising Audio Codec", "INIT", ERROR);
        return 1;
    }

    SCREEN_W = 512;
    SCREEN_H = 512;
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        Util::log("Error creating display", "INIT", ERROR);
        return 1;
    }
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    al_set_display_icon(display, al_load_bitmap("resources/icon.png"));
    al_set_window_title(display, "FCW the RPG");

    Util::log("Engine initialized successfully");

    Util::log("Initializing Game");
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

    Music::init();

    //Map::test();
    current_map = Map::loadMap("map_test");
    current_map->setEventHandlerFunction(mapEventHandler);

    music1 = al_create_sample_instance(current_map->music[0]);
    al_set_sample_instance_playmode(music1, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(music1, 1.0);

    music2 = al_create_sample_instance(current_map->music[1]);
    al_set_sample_instance_playmode(music2, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(music2, 1.0);

    hat = current_map->getSpriteById("s_hat");
    hat->setX(SCREEN_W/4.0f -(hat->width/2.0f));
    hat->setY(SCREEN_H/4.0f -(hat->height/2.0f));
    hat->clickAction = clickFunction;

    Music::playMusic(music1);

    Util::log("Initialization Finished, Starting Game");
    oldTime = al_get_time();
    while (!done) {
        update();
    }
}