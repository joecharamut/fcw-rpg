

#include "Keyboard.h"
#include "Util.h"

bool Keyboard::keyStates[];

int Keyboard::init() {
    for (bool &keyState : keyStates) {
        keyState = false;
    }

    return 1;
}

void Keyboard::registerKeyEvent(ALLEGRO_EVENT event) {
    if (event.type == ALLEGRO_EVENT_KEY_UP) {
        keyStates[event.keyboard.keycode] = false;
    } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        keyStates[event.keyboard.keycode] = true;
    }
}

bool Keyboard::getKeyState(Keycode k) {
    return keyStates[k];
}

/*{
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
                //if (temp) Music::playMusic(music1); else Music::playMusic(music2);
                temp = !temp;
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
    }*/