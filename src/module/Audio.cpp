
#include "Util.h"
#include "module/Audio.h"
#include "Log.h"

ALLEGRO_MIXER *Audio::musicMixer;
ALLEGRO_MIXER *Audio::sfxMixer;
ALLEGRO_MIXER *Audio::masterMixer;
ALLEGRO_SAMPLE_INSTANCE *Audio::playing;

int Audio::init() {
    // Reserve audio samples
    if (!al_reserve_samples(4)) {
        Log::error("Error reserving samples");
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
bool Audio::switching = false;

// Current state
State Audio::state = STATE_NONE;
// Music gain
float Audio::gain = 1.0;

// Next music
ALLEGRO_SAMPLE_INSTANCE *Audio::next;

void Audio::update() {
    // If we are switching
    if (switching) {
        // If the state is NONE and gain is 1.0, set state FADE_OUT
        if (state == STATE_NONE && gain == 1.0) state = STATE_FADEOUT;
        // If state FADE_OUT and gain is <= 0 (muted), set state SWITCHING
        else if (state == STATE_FADEOUT && gain <= 0.0) state = STATE_SWITCHING;
        // Set state FADE_IN if state is SWITCHING
        else if (state == STATE_SWITCHING) state = STATE_FADEIN;
        // If FADE_IN and gain >= 1.0 set state NONE, switching false and gain 1.0
        else if (state == STATE_FADEIN && gain >= 1.0) {
            state = STATE_NONE;
            switching = false;
            gain = 1.0;
        }
        // For every loop
        switch (state) {
            // If the state is FADE_OUT
            case STATE_FADEOUT:
                // Subtract 0.033 from gain
                gain -= 0.033;
                break;
            case STATE_FADEIN:
                // Add 0.033 to gain
                gain += 0.033;
                break;
            case STATE_SWITCHING:
                // Stop last playing
                if (playing) al_set_sample_instance_playing(playing, false);
                if (playing) al_detach_sample_instance(playing);
                // Set new playing
                playing = next;
                // Attach to mixer
                al_attach_sample_instance_to_mixer(playing, musicMixer);
                al_set_sample_instance_position(playing, 0);
                al_set_sample_instance_playing(playing, true);
                break;
            default: break;
        }
        // Set gain
        al_set_mixer_gain(musicMixer, gain);
    }
}

void Audio::playMusic(ALLEGRO_SAMPLE_INSTANCE *music) {
    // Set next music
    next = music;
    // Trigger switch
    switching = true;
}

void Audio::playSFX(ALLEGRO_SAMPLE_INSTANCE *sfx) {

}

std::map<std::string, ALLEGRO_SAMPLE_INSTANCE *> Audio::registeredMusic = {};
std::map<std::string, ALLEGRO_SAMPLE_INSTANCE *> Audio::registeredSFX = {};

bool Audio::registerMusic(ALLEGRO_SAMPLE_INSTANCE *reg, std::string id) {
    if (registeredMusic.count(id)) return false;
    registeredMusic[id] = reg;
    return true;
}