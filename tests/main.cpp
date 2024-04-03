#define IMAGE_ENABLED

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

	Window window("Graphics Demo", {800, 600});
	Renderer renderer(window);

	Clock clock;
	Events events;

	Texture tex(renderer, "../SDL_Logo.png");
	Rect tex_rect = tex.get_rect();
	tex_rect.center({400, 350});

	Font font("../font.ttf", 40);

	string text = "SDL with C++ is cool!";
	Texture text_tex = font.create_text(renderer, text, ORANGE);
	Rect text_rect = text_tex.get_rect();
	text_rect.center({400, 500});

	FontAtlas font_atlas = font.create_atlas(renderer, GREEN);

	bool running = true;
	double dt;

	EVENT_KEYS["UP"] = {SDLK_w, SDLK_UP};
	EVENT_KEYS["QUIT"] = {SDLK_ESCAPE};

	while (running) {
		dt = clock.tick(60);

		running = events.process_events(&EVENT_KEYS);

		if (EVENT_KEYS["UP"].pressed)
			SDL_Log("Pressed UP");
		if (EVENT_KEYS["UP"].down)
			SDL_Log("Held UP");
		if (EVENT_KEYS["UP"].released)
			SDL_Log(" Released UP");
		if (EVENT_KEYS["QUIT"].pressed)
			running = false;

		renderer.clear(WHITE);
		renderer.draw_rect({100, 100, 100, 100}, RED);
		renderer.draw_rect({250, 100, 100, 100}, YELLOW, 1);
		renderer.draw_rect({400, 100, 100, 100}, BLUE, 10);
		renderer.draw_line({550, 100}, {650, 200}, GREEN);
		renderer.draw_line({700, 100}, {800, 200}, GRAY, 4);
		renderer.draw_circle({700, 150, 50}, PURPLE, false);
		renderer.draw_circle({650, 350, 50}, {255, 100, 30});
		renderer.draw_polygon({{50, 300}, {150, 250}, {250, 350}, {150, 390}}, {10, 230, 50, 100});
		renderer.draw_polygon({{50, 450}, {150, 450}, {250, 500}, {150, 550}, {50, 500}}, {200, 30, 45}, false);
		tex.render(tex_rect);
		text_tex.render(text_rect);
		font_atlas.draw_text("FPS: " + to_string(int(clock.get_fps())), {1, 1}, {100, 250, 30});
		renderer.present();
	}

	return 0;
}
