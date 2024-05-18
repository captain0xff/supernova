#include "mixer.h"

#include "logging.h"



// Globals
static int MUSIC_ID = 0;
static int SOUND_ID = 0;



// Classes
Mixer::Mixer() {
	if (Mix_OpenAudio(0, NULL) < 0)
		flog_error("Failed to open audio device: {}", Mix_GetError());
}

Mixer::~Mixer() {
	Mix_CloseAudio();
}

void Mixer::allocate_channels(int channels) {
	Mix_AllocateChannels(channels);
}


Music::Music(const string &file): music(managed_ptr<Mix_Music>(Mix_LoadMUS(file.c_str()), Mix_FreeMusic)) {
	if (music == NULL) {
		flog_error("Failed to load music! ({}): {}", file, Mix_GetError());
	} else {
		id = MUSIC_ID;
		flog_info("Music loaded successfully![{}] ({})", id, file);
		MUSIC_ID++;
	}
}

void Music::play(int loop) {
	Mix_PlayMusic(music.get(), loop);
}

float Music::volume() {
	// Returns the current volume
	return static_cast<float>(Mix_VolumeMusic(-1))/MIX_MAX_VOLUME;
}

void Music::volume(float volume) {
	// The value of the parameter volume should be between 0 to 1
	Mix_VolumeMusic((int)(volume*MIX_MAX_VOLUME));
}

void Music::pause() {
	if (is_paused) {
		flog_warn("Music is already paused!");
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
		flog_warn("Music is not paused!");
	}
}

void Music::toggle() {
	if (is_paused)
		Mix_ResumeMusic();
	else
		Mix_PauseMusic();
	is_paused = !is_paused;
}


Sound::Sound(const string file): sound(managed_ptr<Mix_Chunk>(Mix_LoadWAV(file.c_str()), Mix_FreeChunk)) {
	if (sound == NULL) {
		flog_error("Failed to load sound! ({}): {}", file, Mix_GetError());
	} else {
		id = SOUND_ID;
		flog_info("Sound loaded successfully![{}] ({})", id, file);
		SOUND_ID++;
	}
}

void Sound::play(int loop, int channel) {
	// Pass -1 to the loop for looping infinitely
	// The first free channel is choosed by default
	if ((channel = Mix_PlayChannel(-1, sound.get(), loop)) < 0) {
		flog_error("Failed to play sound![{}]: {}", id, Mix_GetError());
	}
}

float Sound::volume() {
	// Returns the current volume
	return static_cast<float>(Mix_Volume(channel, -1))/MIX_MAX_VOLUME;
}

void Sound::volume(float volume) {
	// The value of the parameter volume should be between 0 to 1
	Mix_Volume(channel, (int)(volume*MIX_MAX_VOLUME));
}

void Sound::pause() {
	if (is_paused) {
		flog_warn("Sound is already paused!");
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
		flog_warn("Sound is not paused!");
	}
}

void Sound::toggle() {
	if (is_paused)
		Mix_Resume(channel);
	else
		Mix_Pause(channel);
	is_paused = !is_paused;
}
