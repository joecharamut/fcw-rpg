
#include <allegro5/allegro_ttf.h>
#include "BaseMenu.h"
#include "Globals.h"

BaseMenu::BaseMenu() {
    this->texts = new LinkedTextSprite();
    this->buttons = new LinkedTextButton();
    this->sprites = new LinkedSprite();
}



void BaseMenu::draw() {
    LinkedTextSprite *nextText = texts;
    while(nextText != nullptr) {
        if (nextText->text != NULL)
            al_draw_text(font16, al_map_rgb(0,0,0), nextText->x, nextText->y, 0, nextText->text);
        nextText = nextText->next;
    }
    LinkedTextButton *nextButton = buttons;
    while(nextButton != nullptr) {
        if (nextButton->text != NULL)
            al_draw_text(font16, al_map_rgb(0,0,0), nextButton->x, nextButton->y, 0, nextButton->text);
        nextButton = nextButton->next;
    }
    LinkedSprite *nextSprite = sprites;
    while(nextSprite != nullptr) {
        if (nextSprite->sprite != NULL)
            nextSprite->sprite->draw();
        nextSprite = nextSprite->next;
    }
}

void BaseMenu::addText(const char *text, float x, float y) {
    LinkedTextSprite *add = new LinkedTextSprite();
    add->text = text;
    add->x = x;
    add->y = y;

    LinkedTextSprite *next = texts;
    if (!next) {
        next = new LinkedTextSprite();
    }
    while(next->next != nullptr) {
        next = next->next;
    }
    next->next = add;
}

void BaseMenu::addButton(const char *text, float x, float y, void *handler) {
    LinkedTextButton *add = new LinkedTextButton();
    add->text = text;
    add->x = x;
    add->y = y;
    add->handler = handler;

    LinkedTextButton *next = buttons;
    while(next->next != nullptr) {
        next = next->next;
    }
    next->next = add;
}

void BaseMenu::addSprite(Sprite *sprite) {
    LinkedSprite *add = new LinkedSprite();
    add->sprite = sprite;

    LinkedSprite *next = sprites;
    while(next->next != nullptr) {
        next = next->next;
    }
    next->next = add;
}

void BaseMenu::handleEvent(ALLEGRO_EVENT event) {

}
