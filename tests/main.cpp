#include "../src/core.h"
#include "../src/constants.h"


using namespace std;



int main(int argc, char* argv[]) {

	Engine engine;

	Window window("", 800, 600);
	Renderer renderer(window);

	Clock clock;
	Events events;

	bool running = true;
	double dt;

	Mouse mouse(MB::LEFT|MB::RIGHT);

	Colour rect_colour = {0, 0, 0};

	while (running) {
		dt = clock.tick(60);

		running = events.process_events(&EVENT_KEYS, &mouse);

		if (mouse.buttons[MB::LEFT].down)
			rect_colour.r = 255;
		else
			rect_colour.r = 0;

		if (mouse.buttons[MB::RIGHT].down)
			rect_colour.g = 255;
		else
			rect_colour.g = 0;

		renderer.clear(WHITE);
		renderer.draw_rect({mouse.pos, {100, 100}}, rect_colour);
		renderer.present();
	}

	return 0;
}
