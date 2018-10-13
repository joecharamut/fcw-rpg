#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include "sprite.cpp"

const float FPS = 60;
const int SCREEN_H = 600;
const int SCREEN_W = 800;

enum KEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

struct Node {
    sprite *sprite;
    struct Node *next;
};

bool done = false;
bool redraw = true;
int hat_x = 0;
int hat_y = 0;
bool key[4] = { false, false, false, false };
Node *background;
Node *pSprite;
ALLEGRO_EVENT_QUEUE *queue;

void handleEvents() {
    ALLEGRO_EVENT event;
    al_wait_for_event(queue, &event);
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
        pSprite->sprite->setX(hat_x);
        pSprite->sprite->setY(hat_y);
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
    }
    else if(event.type == ALLEGRO_EVENT_KEY_UP) {
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
        Node *next = background;
        while (next != NULL) {
            next->sprite->draw();
            next = next->next;
        }
        al_flip_display();
    }
}

void loadSprites() {
    ALLEGRO_BITMAP *hatImage = al_load_bitmap("res/hat.png");
    pSprite = (Node*) malloc(sizeof(Node));
    pSprite->sprite = new sprite(0,0,hatImage);
    pSprite->next = NULL;

    ALLEGRO_BITMAP *bg01 = al_load_bitmap("res/bars.png");
    background = (Node*) malloc(sizeof(Node));
    background->sprite = new sprite(0,0,bg01);
    background->next = pSprite;
}

int main(int argc, char *argv[]) {
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    al_start_timer(timer);

    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);

    ALLEGRO_FONT *font = al_load_ttf_font("res/OpenSans.ttf", 48, 0);
    if (!font) {
        printf("Error in font load!");
        return -1;
    }

    loadSprites();

    while (!done) {
        update();
    }
}