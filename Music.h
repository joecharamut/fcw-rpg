

#ifndef FCWRPG_MUSIC_H
#define FCWRPG_MUSIC_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

class Music {
public:
    static ALLEGRO_MIXER *musicMixer;
    static ALLEGRO_MIXER *sfxMixer;
    static ALLEGRO_MIXER *masterMixer;

    static void init();
    static void update();
    static void playSFX(ALLEGRO_SAMPLE_INSTANCE *sfx);
    static void playMusic(ALLEGRO_SAMPLE_INSTANCE *music);
};


#endif //FCWRPG_MUSIC_H
