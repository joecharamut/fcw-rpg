#ifndef FCWRPG_GUICOMPONENTGRAPHICS_H
#define FCWRPG_GUICOMPONENTGRAPHICS_H


#include <allegro5/bitmap.h>
#include <gui/GuiComponent.h>

class GuiComponentGraphics : public GuiComponent {
public:

    GuiComponentGraphics(int x, int y, int width, int height, unsigned char r, unsigned char g, unsigned char b);
    ~GuiComponentGraphics() override = default;

    ALLEGRO_BITMAP *getRendered() override;

private:
    ALLEGRO_BITMAP *graphicsBuffer;
};


#endif //FCWRPG_GUICOMPONENTGRAPHICS_H
