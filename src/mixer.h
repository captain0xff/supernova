#ifndef SUPERNOVA_MIXER_H
#define SUPERNOVA_MIXER_H

#ifndef MIXER_ENABLED
#define MIXER_ENABLED
#endif /* MIXER_ENABLED */


#include <SDL3_mixer/SDL_mixer.h>

#include "core.h"



// Classes
class Mixer {
public:
	managed_ptr<MIX_Mixer> mixer;

	Mixer(SDL_AudioDeviceID device_id=SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK);
};


class Audio {
public:
	int id;
	managed_ptr<MIX_Audio> audio;

	// Currently only supports WAV format
	Audio(Mixer &mixer, const string &file, bool predecode=true);	
};


class Track {
public:
	int id;
	bool is_paused;
	managed_ptr<MIX_Track> track;

	Track(Mixer &mixer);

	// Pass -1 to the loop for looping infinitely
	void play(int loop=0);
	void play(SDL_PropertiesID options);
	float volume();
	// The value of the parameter volume should be between 0 to 1
	void volume(float volume);
	void pause();
	void resume();
	// Toogles the music playing i.e resumes if paused and vice-versa
	void toggle();

	void setAudio(Audio &audio);
};


#endif /* SUPERNOVA_MIXER_H */
