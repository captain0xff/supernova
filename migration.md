# Migration Guide
## TODO
* Expose new SDL3 functionality (Properties of Renderer and Texture).
* Take another shot at making a better event system.
* Upgrade the satellite libs (done for now).
* Wrap SDL_OpenAudioDevice as a constructor of the mixer class.
* Add AnimatedSprite::render_rot

## Already done
* Renamed functions and variables.
* Changed render_ex to render_rot.
* Modified the primitive classes to use float instead of doubles.
* Introduce FRect and think if keeping Rect will be needed.
* Change Circle class to use floats.
* Change the SDL int types to C fixed width ints.
* After SDL3 is released rename supernova3 to supernova and the sdl2 branch to supernova2.
