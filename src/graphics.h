#ifndef SUPERNOVA_GRAPHICS_H
#define SUPERNOVA_GRAPHICS_H

#ifndef IMAGE_ENABLED
#define IMAGE_ENABLED
#endif /* IMAGE_ENABLED */


#include "core.h"


using namespace std;



// Classes
class SpriteSheet {
	public:
		int tile_x, tile_y, tile_w, tile_h, total_tiles;
		Rect src_rect;
		Texture texture;

		SpriteSheet(Renderer &renderer, const string &file, const int &column, const int &row);

		void set_src_rect(const Rect &src_rect);
		void draw_sprite(const Rect &dst_rect, const int &column, const int &row);
		// void destroy();
};


class AnimatedSprite: public SpriteSheet {
	public:
		bool loop;
		double animation_speed; // In animation_frames/sec
		double animation_index = 0;

		AnimatedSprite(Renderer &renderer, const string &file, const int &column, const int &row, const double animation_speed, bool loop=true);

		// Returns true when the animation has just completed looping once
		bool update(double dt);
		void render(const Rect &dst_rect);
		void render_ex(const Rect &dst_rect, const double &angle=0, const Vector &center={0, 0}, const SDL_RendererFlip &flip=SDL_FLIP_NONE);
};

#endif /* SUPERNOVA_GRAPHICS_H */
