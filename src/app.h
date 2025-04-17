#ifndef SUPERNOVA_APP_H
#define SUPERNOVA_APP_H


#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <supernova/core.h>

class App {
public:
	SDL_AppResult result = SDL_APP_CONTINUE;

	App(int, char**) {};

	virtual void handle_events(SDL_Event*) {};
	virtual void iterate() {};
};

class SApp: public App {
public:
	float fps = 0.0f;
	// If true this will stop the application on SDL_EVENT_QUIT
	bool stop_on_quit = true;

	Clock clock;
	Events events;

	EventKeys event_keys;
	Mouse mouse;

	SApp(int argc, char **argv): App(argc, argv) {
		event_keys = {
			{"UP",     {SDLK_W, SDLK_UP   }},
			{"DOWN",   {SDLK_S, SDLK_DOWN }},
			{"LEFT",   {SDLK_A, SDLK_LEFT }},
			{"RIGHT",  {SDLK_D, SDLK_RIGHT}},
			{"ACTION", {SDLK_RETURN       }},
			{"QUIT",   {SDLK_ESCAPE       }}
		};
	};

	// If this function returns true then the input
	// states are not processed further
	virtual bool process_events(SDL_Event&) {return true;};
	virtual void update(double) {};
	virtual void draw() {};

	void handle_events(SDL_Event *event) override {
		if (!process_events(*event))
			return;

		switch (event->type) {
			case SDL_EVENT_QUIT:
				if (stop_on_quit) {
					result = SDL_APP_SUCCESS;
				}
				break;
			case SDL_EVENT_KEY_DOWN:
				if (!event->key.repeat) {
					for (auto &[key, value]: event_keys) {
						if ((event->key.key == value.primary) || (event->key.key == value.secondary)) {
							value.pressed = true;
							value.down = true;
						}
					}
				}
				break;
			case SDL_EVENT_KEY_UP:
				if (!event->key.repeat) {
					for (auto &[key, value]: event_keys) {
						if ((event->key.key == value.primary) || (event->key.key == value.secondary)) {
							value.released = true;
							value.down = false;
						}
					}
				}
				break;
			case SDL_EVENT_MOUSE_MOTION:
				mouse.pos.x = event->motion.x;
				mouse.pos.y = event->motion.y;
				mouse.rel_pos.x = event->motion.xrel;
				mouse.rel_pos.y = event->motion.yrel;
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				for (auto &[key, value]: mouse.buttons) {
					if (event->button.button == value.id) {
						value.pressed = true;
						value.down = true;
					}
				}
				break;
			case SDL_EVENT_MOUSE_BUTTON_UP:
				for (auto &[key, value]: mouse.buttons) {
					if (event->button.button == value.id) {
						value.released = true;
						value.down = false;
					}
				}
				break;
		}
	}

	void iterate() override {
		double dt = (fps)? clock.tick(fps) : clock.tick();
		update(dt);
		draw();
		reset_input_states();
	}

private:
	void reset_input_states() {
		// For keyboard keys
		for (auto &[key, value]: event_keys) {
			value.pressed = false;
			value.released = false;
		}

		// For mouse
		for (auto &[key, value]: mouse.buttons) {
			value.pressed = false;
			value.released = false;
		}
		mouse.vert_wheel = mouse.horz_wheel = 0;
		mouse.rel_pos = {0, 0};
	}
};

#define SUPERNOVA_APP_MAIN(classname) \
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) { 		\
	classname *app = new classname(argc, argv); 							\
	*appstate = app; 														\
\
	return app->result;														\
}																			\
\
SDL_AppResult SDL_AppIterate(void *appstate) { 								\
	classname *app = (classname *)appstate; 								\
	app->iterate(); 														\
\
	return app->result; 													\
} 																			\
\
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { 				\
	classname *app = (classname *)appstate; 								\
	app->handle_events(event); 												\
\
	return app->result; 													\
}																			\
\
void SDL_AppQuit(void *appstate, SDL_AppResult result) { 					\
	classname *app = (classname *)appstate; 								\
	delete app; 															\
}																			\

#endif /* SUPERNOVA_APP_H */
