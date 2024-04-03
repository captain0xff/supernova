#ifndef SUPERNOVA_MIXER_H
#define SUPERNOVA_MIXER_H

#ifndef MIXER_ENABLED
#define MIXER_ENABLED
#endif /* MIXER_ENABLED */


#include <SDL3_mixer/SDL_mixer.h>

#include "core.h"



// Forward Declarations
class Mixer;
class Music;
class Sound;



// Classes
class Mixer {
	public:
		void static open_audio_device(int frequency=MIX_DEFAULT_FREQUENCY, Uint16 format=MIX_DEFAULT_FORMAT, int channels=2, int chunksize=2048);
		void static allocate_channels(int channels);
};


extern int MUSIC_ID;
class Music {
	public:
		int id;
		bool is_paused = false;
		managed_ptr<Mix_Music> music;

		Music(const string &file);

		void play(int loop=0);
		// Returns the current volume
		float volume();
		// The value of the parameter volume should be between 0 to 1
		void volume(float volume);
		void pause();
		void resume();
		// Toogles the music playing i.e resumes if paused and vice-versa
		void toggle();
		// void destroy();
};


extern int SOUND_ID;
class Sound {
	public:
		int id, channel;
		bool is_paused;
		managed_ptr<Mix_Chunk> sound;

		// Currently only supports WAV format
		Sound(const string file);

		// Pass -1 to the loop for looping infinitely
		// The first free channel is choosed by default
		void play(int loop=0, int channel=-1);
		// Returns the current volume
		float volume();
		// The value of the parameter volume should be between 0 to 1
		void volume(float volume);
		void pause();
		void resume();
		// Toogles the music playing i.e resumes if paused and vice-versa
		void toggle();
		// void destroy();		
};


#endif /* SUPERNOVA_MIXER_H */
