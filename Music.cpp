
#include "Util.h"
#include "Music.h"

ALLEGRO_MIXER *Music::musicMixer;
ALLEGRO_MIXER *Music::sfxMixer;
ALLEGRO_MIXER *Music::masterMixer;
ALLEGRO_SAMPLE_INSTANCE *Music::playing;

int Music::init() {
    // Reserve audio samples
    if (!al_reserve_samples(4)) {
        Util::log("Error reserving samples", "Audio", ERROR);
        return 0;
    }
    // Create mixers
    masterMixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    musicMixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    sfxMixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);

    // Attach mixers
    // (Music + SFX) -> master -> output
    al_attach_mixer_to_mixer(musicMixer, masterMixer);
    al_attach_mixer_to_mixer(sfxMixer, masterMixer);
    al_attach_mixer_to_mixer(masterMixer, al_get_default_mixer());
    return 1;
}

// Music switching flag
bool switching = false;
// Fade progress states
enum State {
    NONE,
    FADE_OUT,
    SWITCHING,
    FADE_IN
};
// Current state
State state = NONE;
// Music gain
float gain = 1.0;

// Next music
ALLEGRO_SAMPLE_INSTANCE *next;

void Music::update() {
    // If we are switching
    if (switching) {
        // If the state is NONE and gain is 1.0, set state FADE_OUT
        if (state == NONE && gain == 1.0) state = FADE_OUT;
        // If state FADE_OUT and gain is <= 0 (muted), set state SWITCHING
        else if (state == FADE_OUT && gain <= 0.0) state = SWITCHING;
        // Set state FADE_IN if state is SWITCHING
        else if (state == SWITCHING) state = FADE_IN;
        // If FADE_IN and gain >= 1.0 set state NONE, switching false and gain 1.0
        else if (state == FADE_IN && gain >= 1.0) {
            state = NONE;
            switching = false;
            gain = 1.0;
        }
        // For every loop
        switch (state) {
            // If the state is FADE_OUT
            case FADE_OUT:
                // Subtract 0.033 from gain
                gain -= 0.033;
                break;
            case FADE_IN:
                // Add 0.033 to gain
                gain += 0.033;
                break;
            case SWITCHING:
                // Stop last playing
                if (playing) al_set_sample_instance_playing(playing, false);
                if (playing) al_detach_sample_instance(playing);
                // Set new playing
                playing = next;
                // Attach to mixer
                al_attach_sample_instance_to_mixer(playing, musicMixer);
                al_set_sample_instance_playing(playing, true);
                break;
            default: break;
        }
        // Set gain
        al_set_mixer_gain(musicMixer, gain);
    }
}

bool Music::registerMusic(ALLEGRO_SAMPLE_INSTANCE *reg, std::string id) {
    if (Music::registeredMusic.count(id)) return false;
    Music::registeredMusic[id] = reg;
    return true;
}

void Music::playMusic(ALLEGRO_SAMPLE_INSTANCE *music) {
    // Set next music
    next = music;
    // Trigger switch
    switching = true;
}

void Music::playSFX(ALLEGRO_SAMPLE_INSTANCE *sfx) {

}


