
#include "Util.h"
#include "Music.h"

ALLEGRO_MIXER *Music::musicMixer;
ALLEGRO_MIXER *Music::sfxMixer;
ALLEGRO_MIXER *Music::masterMixer;

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

void Music::update() {

}

void Music::playSFX(ALLEGRO_SAMPLE_INSTANCE *sfx) {

}

void Music::playMusic(ALLEGRO_SAMPLE_INSTANCE *music) {
    al_attach_sample_instance_to_mixer(music, musicMixer);
}
