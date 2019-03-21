#include "Keyboard.h"
#include "../Util.h"

bool Keyboard::keyStates[];

bool Keyboard::keyBufferEnabled = false;
std::vector<char> Keyboard::keyBuffer;

bool Keyboard::init() {
    for (bool &keyState : keyStates) {
        keyState = false;
    }

    return true;
}

void Keyboard::registerKeyEvent(ALLEGRO_EVENT event) {
    if (event.type == ALLEGRO_EVENT_KEY_UP) {
        keyStates[event.keyboard.keycode] = false;
    } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        keyStates[event.keyboard.keycode] = true;
    }
    if (keyBufferEnabled) {
        if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            keyBuffer.push_back(((char) event.keyboard.unichar));
        }
    }
}

bool Keyboard::getKeyState(Keycode k) {
    return keyStates[k];
}

void Keyboard::setKeyBuffer(bool state) {
    keyBufferEnabled = state;
}

char Keyboard::consumeKey() {
    if (!keyBuffer.empty()) {
        char ret = keyBuffer.back();
        keyBuffer.pop_back();
        return ret;
    }
    return '\0';
}