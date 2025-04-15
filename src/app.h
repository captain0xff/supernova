#ifndef SUPERNOVA_APP_H
#define SUPERNOVA_APP_H


#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <supernova/core.h>

class App {
public:
	SDL_AppResult result;

	App(int, char**) {};

	virtual void handle_events(SDL_Event*) {};
	virtual void iterate() {};
};

class SApp: public App {
public:
	float fps = 60.0f;

	Clock clock;
	Events events;

	SApp(int argc, char **argv): App(argc, argv) {};

	virtual void update(double) {};
	virtual void draw() {};

	void iterate() override {
		double dt = clock.tick(fps);
		update(dt);

		draw();
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
