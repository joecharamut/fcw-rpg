#ifndef FCWRPG_MUSIC_H
#define FCWRPG_MUSIC_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <string>
#include <map>

// Fade progress states
enum State {
    STATE_NONE,
    STATE_FADEOUT,
    STATE_SWITCHING,
    STATE_FADEIN
};

class Audio {
public:
    static ALLEGRO_MIXER *musicMixer;
    static ALLEGRO_MIXER *sfxMixer;
    static ALLEGRO_MIXER *masterMixer;
    static ALLEGRO_SAMPLE_INSTANCE *playing;

    static bool switching;
    static State state;
    static float gain;
    static ALLEGRO_SAMPLE_INSTANCE *next;

    static std::map<std::string, ALLEGRO_SAMPLE_INSTANCE *> registeredMusic;
    static std::map<std::string, ALLEGRO_SAMPLE_INSTANCE *> registeredSFX;

    static int init();
    static void update();

    static bool registerMusic(ALLEGRO_SAMPLE_INSTANCE *reg, std::string id);
    static bool registerSFX(ALLEGRO_SAMPLE_INSTANCE *reg, std::string id);

    static void playMusic(ALLEGRO_SAMPLE_INSTANCE *music);
    static void playMusic(std::string id);

    static void playSFX(ALLEGRO_SAMPLE_INSTANCE *sfx);
    static void playSFX(std::string id);
};


#endif //FCWRPG_MUSIC_H
