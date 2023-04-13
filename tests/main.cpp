#include "..\src\engine.h"

using namespace std;



int main(int, char**) {

	Engine engine;

	Window window("test", 800, 600);
	Renderer renderer(window);

	Clock clock;
	Events events;

	bool running = true;
	double dt, volume;

	Mouse mouse({"LEFT", "RIGHT"});

	Font font("font.ttf", 20);
	Texture static_text = font.create_text(renderer, "This is very cool\nI like it", {100, 240, 37}, 1, true);
	Rect static_text_rect = static_text.get_rect();
	static_text_rect.topleft({0, 0});
	FontAtlas atlas(font.create_atlas(renderer, {0, 255, 0, 255}));

	while (running) {
		dt = clock.tick(60);

		running = events.process_events(&EVENT_KEYS, &mouse);

		renderer.clear({255, 0, 0});
		// static_text.render(static_text_rect);
		atlas.draw_text("Hello everyone how are you?", {1, 1});
		renderer.present();
	}

	return 0;
}
