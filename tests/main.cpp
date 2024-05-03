#include <SDL3/SDL_main.h>

#include "../src/core.h"
#include "../src/constants.h"
#include "../src/networking.h"
#include "../src/graphics.h"
#include "../src/logging.h"
#include "../src/mixer.h"
#include "../src/font.h"
#include "../src/print.h"


using namespace std;



int main(int argc, char *argv[]) {
	Engine engine;

	Window window("Camera Demo", {800, 600});
	Renderer renderer(window);

	Clock clock;
	Events events;

	bool rng = true;
	double dt;

	Camera camera;

	Texture tex(renderer, camera.size, camera.format, SDL_TEXTUREACCESS_STREAMING);

	while (rng) {
		dt = clock.tick(60);

		rng = events.process_events(&EVENT_KEYS);

		if ((camera.get_permission_state() == Camera::APPROVED)) {
			if (camera.is_new_frame_available()) {
				tex.update(camera.acquire_frame());
				camera.release_frame();
			}
		}

		renderer.clear(WHITE);
		tex.render({0, 0, 800, 600});
		renderer.present();
	}

	return 0;
}
