
#include "Util.h"
#include "Music.h"

ALLEGRO_MIXER *Music::musicMixer;
ALLEGRO_MIXER *Music::sfxMixer;
ALLEGRO_MIXER *Music::masterMixer;
ALLEGRO_SAMPLE_INSTANCE *Music::playing;

void Music::init() {
    Util::log("Setting up audio");
    if (!al_reserve_samples(4)) {
        Util::log("Error reserving samples", "Audio", ERROR);
    }
    masterMixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    musicMixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    sfxMixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);

    al_attach_mixer_to_mixer(musicMixer, masterMixer);
    al_attach_mixer_to_mixer(sfxMixer, masterMixer);
    al_attach_mixer_to_mixer(masterMixer, al_get_default_mixer());
    Util::log("Done");
}

bool switching = false;
enum State {
    NONE,
    FADE_OUT,
    SWITCHING,
    FADE_IN
};
State state = NONE;
float gain = 1.0;

ALLEGRO_SAMPLE_INSTANCE *next;

void Music::update() {
    if (switching) {
        if (state == NONE && gain == 1.0) {
            state = FADE_OUT;
        } else if (state == FADE_OUT && gain <= 0.0) {
            state = SWITCHING;
        } else if (state == SWITCHING) {
            state = FADE_IN;
        } else if (state == FADE_IN && gain >= 1.0) {
            state = NONE;
            switching = false;
        }
        switch (state) {
            case FADE_OUT:
                gain -= 0.05;
                al_set_mixer_gain(musicMixer, gain);
                break;
            case SWITCHING:
                if (playing) al_set_sample_instance_playing(playing, false);
                if (playing) al_detach_sample_instance(playing);
                playing = next;
                al_attach_sample_instance_to_mixer(playing, musicMixer);
                al_set_sample_instance_playing(playing, true);
                break;
            case FADE_IN:
                gain += 0.05;
                al_set_mixer_gain(musicMixer, gain);
                break;
        }
        //Util::log(std::to_string(gain));
    }
}

void Music::playSFX(ALLEGRO_SAMPLE_INSTANCE *sfx) {

}

void Music::playMusic(ALLEGRO_SAMPLE_INSTANCE *music) {
    next = music;
    switching = true;
    //al_attach_sample_instance_to_mixer(music, musicMixer);
}
