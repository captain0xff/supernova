#include <algorithm>

#include "../src/engine.h"

using namespace std;



int main(int, char**) {

	Init();

	Window window("test", 800, 600);
	Renderer renderer(window);

	Clock clock;
	Events events;

	bool running = true;
	double dt, volume;

	Mouse mouse({"LEFT", "RIGHT"});

	Mixer::open_audio_device();
	Mixer::allocate_channels(1);

	Sound sound("sound.wav");
	sound.play(-1); // Loops the sound infinite times
	volume = sound.volume();

	Rect volume_bar_bg{0, 0, 400, 75};
	volume_bar_bg.center({400, 300});

	int volume_bar_fg_length = 380;
	Rect volume_bar_fg{0, 0, volume_bar_fg_length, 55};
	volume_bar_fg.center(volume_bar_bg.center());

	while (running) {
		dt = clock.tick(60);

		running = events.process_events(&EVENT_KEYS, &mouse);

		if (mouse.buttons["LEFT"].pressed)
			sound.toggle();

		if (mouse.vert_wheel != 0) {
			volume += mouse.vert_wheel*0.01;
			if (volume > 1)
				volume = 1;
			else if (volume < 0)
				volume = 0;
			sound.volume(volume);
		}

		volume_bar_fg.w = volume_bar_fg_length*volume;

		renderer.clear({255, 0, 0});
		renderer.draw_rect(volume_bar_bg, {125, 125, 125});
		renderer.draw_rect(volume_bar_fg, {100, 130, 230});
		renderer.present();
	}

	Quit();

	return 0;
}
