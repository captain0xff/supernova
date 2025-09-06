#include "mixer.h"

#include "logging.h"



// Globals
static int TRACK_ID = 0;
static int AUDIO_ID = 0;



// Classes
Mixer::Mixer(SDL_AudioDeviceID device_id): mixer(
	managed_ptr<MIX_Mixer>(
		MIX_CreateMixerDevice(device_id, nullptr), MIX_DestroyMixer
	)
) {
	if (!mixer)
		flog_error("Failed to open audio device: {}", SDL_GetError());
}


Audio::Audio(Mixer &mixer, const string &file, bool predecode): audio(
	managed_ptr<MIX_Audio>(
		MIX_LoadAudio(mixer.mixer.get(), file.c_str(), predecode),
		MIX_DestroyAudio
	)
) {
	if (!audio) {
		flog_error("Failed to load audio! ({}): {}", file, SDL_GetError());
	} else {
		id = AUDIO_ID;
		flog_info("Audio loaded successfully![{}] ({})", id, file);
		AUDIO_ID++;
	}
}


Track::Track(Mixer &mixer): track(
	managed_ptr<MIX_Track>(
		MIX_CreateTrack(mixer.mixer.get()), MIX_DestroyTrack)
	)
{
	if (!track) {
		flog_error("Failed to create track: {}", SDL_GetError());
	} else {
		id = TRACK_ID;
		flog_info("Track created successfully![{}]", id);
		TRACK_ID++;
	}
}

void Track::play(int loop) {
	SDL_PropertiesID options = SDL_CreateProperties();
	SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, loop);

	play(options);
}

void Track::play(SDL_PropertiesID options) {
	MIX_PlayTrack(track.get(), options);
}

float Track::volume() {
	return MIX_GetTrackGain(track.get());
}

void Track::volume(float volume) {
	MIX_SetTrackGain(track.get(), volume);
}

void Track::pause() {
	if (is_paused) {
		flog_warn("Sound is already paused!");
	} else {
		MIX_PauseTrack(track.get());
		is_paused = true;
	}
}

void Track::resume() {
	if (is_paused) {
		MIX_ResumeTrack(track.get());
		is_paused = false;
	} else {
		flog_warn("Sound is not paused!");
	}
}

void Track::toggle() {
	if (is_paused)
		resume();
	else
		pause();
	is_paused = !is_paused;
}

void Track::setAudio(Audio &audio) {
	MIX_SetTrackAudio(track.get(), audio.audio.get());
}
