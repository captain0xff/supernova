#include <SDL3/SDL_main.h>

#include "../src/core.h"
#include "../src/constants.h"


using namespace std;



int main(int argc, char *argv[]) {
	Engine engine;

	Window window("", {800, 600});
	Renderer renderer(window);

	Clock clock;
	Events events;

	bool rng = true;
	double dt;

	Camera camera;

	bool camera_approved = false;

	Texture tex2(renderer, {800, 600});

	while (rng) {
		dt = clock.tick(60);

		rng = events.process_events(&EVENT_KEYS);

		if (!camera_approved) {
			switch (camera.get_permission_state()) {
				case 1:
					camera_approved = true;
					break;
				case -1:
					rng = false;
					break;
			}
		}

		renderer.clear(WHITE);
		renderer.set_target(tex2);
		if (camera_approved && camera.is_new_frame_available()) {
			Texture tex1(renderer, camera.acquire_frame());
			tex1.render({0, 0, 800, 600});
			camera.release_frame();
		}
		renderer.set_target();
		tex2.render({0, 0, 800, 600});
		renderer.present();
	}

	return 0;
}
