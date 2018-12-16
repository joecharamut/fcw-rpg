

#ifndef FCWRPG_KEYBOARD_H
#define FCWRPG_KEYBOARD_H

#include <allegro5/keyboard.h>
#include <vector>
#include <string>
#include <map>

typedef int Keycode;

static std::map<std::string, int> stringToKeycode = {
        {"KEY_A", ALLEGRO_KEY_A},
        {"KEY_B", ALLEGRO_KEY_B},
        {"KEY_C", ALLEGRO_KEY_C},
        {"KEY_D", ALLEGRO_KEY_D},
        {"KEY_E", ALLEGRO_KEY_E},
        {"KEY_F", ALLEGRO_KEY_F},
        {"KEY_G", ALLEGRO_KEY_G},
        {"KEY_H", ALLEGRO_KEY_H},
        {"KEY_I", ALLEGRO_KEY_I},
        {"KEY_J", ALLEGRO_KEY_J},
        {"KEY_K", ALLEGRO_KEY_K},
        {"KEY_L", ALLEGRO_KEY_L},
        {"KEY_M", ALLEGRO_KEY_M},
        {"KEY_N", ALLEGRO_KEY_N},
        {"KEY_O", ALLEGRO_KEY_O},
        {"KEY_P", ALLEGRO_KEY_P},
        {"KEY_Q", ALLEGRO_KEY_Q},
        {"KEY_R", ALLEGRO_KEY_R},
        {"KEY_S", ALLEGRO_KEY_S},
        {"KEY_T", ALLEGRO_KEY_T},
        {"KEY_U", ALLEGRO_KEY_U},
        {"KEY_V", ALLEGRO_KEY_V},
        {"KEY_W", ALLEGRO_KEY_W},
        {"KEY_X", ALLEGRO_KEY_X},
        {"KEY_Y", ALLEGRO_KEY_Y},
        {"KEY_Z", ALLEGRO_KEY_Z},
        {"KEY_1", ALLEGRO_KEY_1},
        {"KEY_0", ALLEGRO_KEY_0},
        {"KEY_2", ALLEGRO_KEY_2},
        {"KEY_3", ALLEGRO_KEY_3},
        {"KEY_4", ALLEGRO_KEY_4},
        {"KEY_5", ALLEGRO_KEY_5},
        {"KEY_6", ALLEGRO_KEY_6},
        {"KEY_7", ALLEGRO_KEY_7},
        {"KEY_8", ALLEGRO_KEY_8},
        {"KEY_9", ALLEGRO_KEY_9},
        {"KEY_PAD_0", ALLEGRO_KEY_PAD_0},
        {"KEY_PAD_1", ALLEGRO_KEY_PAD_1},
        {"KEY_PAD_2", ALLEGRO_KEY_PAD_2},
        {"KEY_PAD_3", ALLEGRO_KEY_PAD_3},
        {"KEY_PAD_4", ALLEGRO_KEY_PAD_4},
        {"KEY_PAD_5", ALLEGRO_KEY_PAD_5},
        {"KEY_PAD_6", ALLEGRO_KEY_PAD_6},
        {"KEY_PAD_7", ALLEGRO_KEY_PAD_7},
        {"KEY_PAD_8", ALLEGRO_KEY_PAD_8},
        {"KEY_PAD_9", ALLEGRO_KEY_PAD_9},
        {"KEY_F1", ALLEGRO_KEY_F1},
        {"KEY_F2", ALLEGRO_KEY_F2},
        {"KEY_F3", ALLEGRO_KEY_F3},
        {"KEY_F4", ALLEGRO_KEY_F4},
        {"KEY_F5", ALLEGRO_KEY_F5},
        {"KEY_F6", ALLEGRO_KEY_F6},
        {"KEY_F7", ALLEGRO_KEY_F7},
        {"KEY_F8", ALLEGRO_KEY_F8},
        {"KEY_F9", ALLEGRO_KEY_F9},
        {"KEY_F10", ALLEGRO_KEY_F10},
        {"KEY_F11", ALLEGRO_KEY_F11},
        {"KEY_F12", ALLEGRO_KEY_F12},
        {"KEY_ESCAPE", ALLEGRO_KEY_ESCAPE},
        {"KEY_TILDE", ALLEGRO_KEY_TILDE},
        {"KEY_MINUS", ALLEGRO_KEY_MINUS},
        {"KEY_EQUALS", ALLEGRO_KEY_EQUALS},
        {"KEY_BACKSPACE", ALLEGRO_KEY_BACKSPACE},
        {"KEY_TAB", ALLEGRO_KEY_TAB},
        {"KEY_OPENBRACE", ALLEGRO_KEY_OPENBRACE},
        {"KEY_CLOSEBRACE", ALLEGRO_KEY_CLOSEBRACE},
        {"KEY_ENTER", ALLEGRO_KEY_ENTER},
        {"KEY_SEMICOLON", ALLEGRO_KEY_SEMICOLON},
        {"KEY_QUOTE", ALLEGRO_KEY_QUOTE},
        {"KEY_BACKSLASH", ALLEGRO_KEY_BACKSLASH},
        {"KEY_COMMA", ALLEGRO_KEY_COMMA},
        {"KEY_FULLSTOP", ALLEGRO_KEY_FULLSTOP},
        {"KEY_SLASH", ALLEGRO_KEY_SLASH},
        {"KEY_SPACE", ALLEGRO_KEY_SPACE},
        {"KEY_INSERT", ALLEGRO_KEY_INSERT},
        {"KEY_DELETE", ALLEGRO_KEY_DELETE},
        {"KEY_HOME", ALLEGRO_KEY_HOME},
        {"KEY_END", ALLEGRO_KEY_END},
        {"KEY_PGUP", ALLEGRO_KEY_PGUP},
        {"KEY_PGDN", ALLEGRO_KEY_PGDN},
        {"KEY_LEFT", ALLEGRO_KEY_LEFT},
        {"KEY_RIGHT", ALLEGRO_KEY_RIGHT},
        {"KEY_UP", ALLEGRO_KEY_UP},
        {"KEY_DOWN", ALLEGRO_KEY_DOWN},
        {"KEY_PAD_SLASH", ALLEGRO_KEY_PAD_SLASH},
        {"KEY_PAD_ASTERISK", ALLEGRO_KEY_PAD_ASTERISK},
        {"KEY_PAD_MINUS", ALLEGRO_KEY_PAD_MINUS},
        {"KEY_PAD_PLUS", ALLEGRO_KEY_PAD_PLUS},
        {"KEY_PAD_DELETE", ALLEGRO_KEY_PAD_DELETE},
        {"KEY_PAD_ENTER", ALLEGRO_KEY_PAD_ENTER},
        {"KEY_PRINTSCREEN", ALLEGRO_KEY_PRINTSCREEN},
        {"KEY_PAUSE", ALLEGRO_KEY_PAUSE},
        {"KEY_LSHIFT", ALLEGRO_KEY_LSHIFT},
        {"KEY_RSHIFT", ALLEGRO_KEY_RSHIFT},
        {"KEY_LCTRL", ALLEGRO_KEY_LCTRL},
        {"KEY_RCTRL", ALLEGRO_KEY_RCTRL},
        {"KEY_ALT", ALLEGRO_KEY_ALT},
        {"KEY_ALTGR", ALLEGRO_KEY_ALTGR},
        {"KEY_LWIN", ALLEGRO_KEY_LWIN},
        {"KEY_RWIN", ALLEGRO_KEY_RWIN},
        {"KEY_MENU", ALLEGRO_KEY_MENU},
        {"KEY_SCROLLLOCK", ALLEGRO_KEY_SCROLLLOCK},
        {"KEY_NUMLOCK", ALLEGRO_KEY_NUMLOCK},
        {"KEY_CAPSLOCK", ALLEGRO_KEY_CAPSLOCK}
};

class Keyboard {
public:

    static int init();
    static void registerKeyEvent(ALLEGRO_EVENT event);
    static bool getKeyState(Keycode k);

private:
    static bool keyStates[ALLEGRO_KEY_MAX];
};


#endif //FCWRPG_KEYBOARD_H
