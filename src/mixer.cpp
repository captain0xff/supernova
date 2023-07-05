#include "mixer.h"


using namespace std;



// Globals
int MUSIC_ID = 0;
int SOUND_ID = 0;



// Classes
void Mixer::open_audio_device(int frequency, Uint16 format, int channels, int chunksize) {
	if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0) {
		SDL_LogError(0, "Failed to open audio device: %s", Mix_GetError());
	}
}

void Mixer::allocate_channels(int channels) {
	Mix_AllocateChannels(channels);
}


Music::Music(const string &file): music(managed_ptr<Mix_Music>(Mix_LoadMUS(file.c_str()), Mix_FreeMusic)) {
	if (music == NULL) {
		SDL_LogError(0, "Failed to load music! (%s): %s", file.c_str(), Mix_GetError());
	} else {
		id = MUSIC_ID;
		SDL_Log("Music loaded successfully![%i] (%s)", id, file.c_str());
		MUSIC_ID++;
	}
}

void Music::play(int loop) {
	Mix_PlayMusic(music.get(), loop);
}

float Music::volume() {
	// Returns the current volume
	return Mix_VolumeMusic(-1)/MIX_MAX_VOLUME;
}

void Music::volume(float volume) {
	// The value of the parameter volume should be between 0 to 1
	Mix_VolumeMusic((int)(volume*MIX_MAX_VOLUME));
}

void Music::pause() {
	if (is_paused) {
		SDL_LogWarn(1, "Music is already paused!");
	} else {
		Mix_PauseMusic();
		is_paused = true;
	}
}

void Music::resume() {
	if (is_paused) {
		Mix_ResumeMusic();
		is_paused = false;
	} else {
		SDL_LogWarn(1, "Music is not paused!");
	}
}

void Music::toggle() {
	if (is_paused)
		Mix_ResumeMusic();
	else
		Mix_PauseMusic();
	is_paused = not is_paused;
}


Sound::Sound(const string file): sound(managed_ptr<Mix_Chunk>(Mix_LoadWAV(file.c_str()), Mix_FreeChunk)) {
	if (sound == NULL) {
		SDL_LogError(0, "Failed to load sound! (%s): %s", file.c_str(), Mix_GetError());
	} else {
		id = SOUND_ID;
		SDL_Log("Sound loaded successfully![%i] (%s)", id, file.c_str());
		SOUND_ID++;
	}
}

void Sound::play(int loop, int channel) {
	// Pass -1 to the loop for looping infinitely
	// The first free channel is choosed by default
	if ((channel = Mix_PlayChannel(-1, sound.get(), loop)) < 0) {
		SDL_LogError(0, "Failed to play sound![%i]: %s", id, Mix_GetError());
	}
}

float Sound::volume() {
	// Returns the current volume
	return Mix_Volume(channel, -1)/MIX_MAX_VOLUME;
}

void Sound::volume(float volume) {
	// The value of the parameter volume should be between 0 to 1
	Mix_Volume(channel, (int)(volume*MIX_MAX_VOLUME));
}

void Sound::pause() {
	if (is_paused) {
		SDL_LogWarn(1, "Sound is already paused!");
	} else {
		Mix_Pause(channel);
		is_paused = true;
	}
}

void Sound::resume() {
	if (is_paused) {
		Mix_Resume(channel);
		is_paused = false;
	} else {
		SDL_LogWarn(1, "Sound is not paused!");
	}
}

void Sound::toggle() {
	if (is_paused)
		Mix_Resume(channel);
	else
		Mix_Pause(channel);
	is_paused = not is_paused;
}
