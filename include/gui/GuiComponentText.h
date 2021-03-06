#ifndef FCWRPG_GUICOMPONENTTEXT_H
#define FCWRPG_GUICOMPONENTTEXT_H


#include <allegro5/bitmap.h>
#include <string>
#include <allegro5/allegro_font.h>
#include <gui/GuiComponent.h>

class GuiComponentText : public GuiComponent {
public:
    bool isSelectable() override { return false; }

    GuiComponentText(std::string text, int x, int y, int width, int height, std::string fontStr,
            unsigned char r, unsigned char g, unsigned char b);
    ~GuiComponentText() override;

    ALLEGRO_BITMAP *getRendered() override;
    void setText(std::string newText);

private:
    ALLEGRO_BITMAP *textBuffer = nullptr;

    std::string text;
    int r,g,b;
    int buf_w, buf_h;
    ALLEGRO_FONT *font;

    void refreshBuffer();
};


#endif //FCWRPG_GUICOMPONENTTEXT_H
