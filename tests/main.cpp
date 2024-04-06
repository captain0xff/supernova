#include <SDL3/SDL_main.h>

#include "../src/core.h"
#include "../src/constants.h"
#include "../src/networking.h"
#include "../src/graphics.h"
#include "../src/logging.h"
#include "../src/mixer.h"
#include "../src/font.h"


using namespace std;



int main(int argc, char *argv[]) {

	Engine engine;

	Window window("Test", {800, 600});
	Renderer renderer(window);

	Clock clock;
	Events events;

	bool rng = true;

	Texture texture(renderer, "../../res/banner.png");
	Rect rect = texture.get_rect();
	rect.scale(800/rect.w);
	rect.center({400, 300});

	while (rng) {
		clock.tick(60);

		rng = events.process_events();

		renderer.clear(WHITE);
		texture.render(rect);
		renderer.present();
	}

	return 0;
}
