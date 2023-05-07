#include <iostream>

#include "../src/engine.h"


using	namespace	std;



class my_class {
	public:
		int a = 3;
		bool event_handler(SDL_Event &event);
};

bool my_class::event_handler(SDL_Event &event) {
	SDL_Log("cool");
	return false;
}


int main(int, char**) {

	// Engine engine;
	Window window("cool", 800, 600);
	Renderer renderer(window);

	bool running = true;

	Events events;
	Clock clock;

	EVENT_KEYS["UP"] = {SDLK_UP};
	Mouse mouse;

	my_class cls;
	// std::function<bool(SDL_Event&)> event_callback = bind(&my_class::event_handler, &cls, placeholders::_1);
}
