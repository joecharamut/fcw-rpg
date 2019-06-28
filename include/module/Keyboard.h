#ifndef FCWRPG_KEYBOARD_H
#define FCWRPG_KEYBOARD_H

#include <allegro5/keyboard.h>
#include <vector>
#include <string>
#include <map>

typedef int Keycode;

class Keyboard {
public:
    static bool init();
    static void registerKeyEvent(ALLEGRO_EVENT event);
    static bool getKeyState(Keycode k);

    static void setKeyBuffer(bool state);
    static std::pair<char, int> consumeKey();
    static std::string consumeKeys(int numKeys);

private:
    static bool keyStates[ALLEGRO_KEY_MAX];

    static bool keyBufferEnabled;
    static std::vector<std::pair<char, int>> keyBuffer;
};


#endif //FCWRPG_KEYBOARD_H
