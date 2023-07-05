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

	while (running) {
		dt = clock.tick(60);

		running = events.process_events(&EVENT_KEYS);

		renderer.clear(WHITE);
		renderer.present();
	}

	return 0;
}
