#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

#include "Sprite.h"
#include "Textbox.h"
#include "Map.h"
#include "BaseMenu.h"
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
LinkedSprite *background;
LinkedSprite *pSprite;
LinkedSprite *textBox;

Map *current_map = nullptr;
BaseMenu *current_menu = nullptr;

ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER *timer;
ALLEGRO_DISPLAY *display;

void handleEvents() {
    ALLEGRO_EVENT event;
    //al_wait_for_event(queue, &event);
    if (!al_get_next_event(queue, &event)) {
        return;
    }
    if (current_map) {
        current_map->handleEvent(event);
        return;
    }
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

void update() {
    handleEvents();
    if(redraw && al_is_event_queue_empty(queue)) {
        redraw = false;
        al_clear_to_color(al_map_rgb(0xff, 0xff, 0xff));
        /*LinkedSprite *next = background;
        while (next != NULL) {
            next->sprite->draw();
            next = next->next;
        }*/
        if (current_map)
            current_map->draw();
        if (current_menu)
            current_menu->draw();
        al_flip_display();
    }
}

void clickFunction(ActionSprite *as) {
    printf("Click\n");
}

void hoverFunction(ActionSprite *as) {
    as->setX(as->x+4.0f);
    as->setY(as->y+4.0f);
    redraw = true;
}

void loadSprites() {
    /*ALLEGRO_BITMAP *textboxTest = al_load_bitmap("resources/paper.png");
    textBox = (LinkedSprite*) malloc(sizeof(LinkedSprite));
    textBox->sprite = new Textbox(0, 344, "Now this is the story all about how\n"
                                  "My life got flipped, turned upside down\n"
                                  "And I'd like to take a minute just sit right there\n"
                                  "I'll tell you how I became the prince of a town called Bel-air",
                                  font32, al_map_rgb(0x00, 0x00, 0x00), textboxTest, nullptr);
    textBox->next = NULL;

    ALLEGRO_BITMAP *bg01 = al_load_bitmap("resources/bars.png");
    background = (LinkedSprite*) malloc(sizeof(LinkedSprite));
    background->sprite = new Sprite(0,0,bg01);
    background->next = pSprite;*/

    ALLEGRO_BITMAP *hatImage = al_load_bitmap("resources/hat.png");

    Tile **tileset = new Tile*[4];
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
    current_map->addSprite(new ActionSprite(0,0,hatImage,"s_hat", clickFunction, hoverFunction));

    //current_menu = new BaseMenu();
    //current_menu->addText("Help me", 0, 0);
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

    al_destroy_font(font8);
    al_destroy_font(font16);
    al_destroy_font(font24);
    al_destroy_font(font32);
    al_destroy_display(display);

}