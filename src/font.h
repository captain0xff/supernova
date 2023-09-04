#ifndef SUPERNOVA_FONT_H
#define SUPERNOVA_FONT_H

#ifndef TTF_ENABLED
#define TTF_ENABLED
#endif /* TTF_ENABLED */


#include <SDL2/SDL_ttf.h>

#include "core.h"


using namespace std;



// Forward Declarations
class Font;
class FontAtlas;



// Classes
extern int FONT_ID;
class Font {
	public:
		int id;
		managed_ptr<TTF_Font> font;

		Font(const string &file, const int size);

		int wrap_alignment();
		void wrap_alignment(const int align);
		Texture create_text(
			Renderer &renderer,
			const string &text,
			const Colour &colour,
			const int &quality=0,
			const bool &wrap_text=true,
			const Uint32 &wrap_length=0,
			const Colour &background_colour={0, 0, 0, 0}
		);
		FontAtlas create_atlas(
			Renderer &renderer,
			const Colour colour={255, 255, 255, 255},
			const int quality=0,
			const string chars="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.:,;'\"(!?)+-*/= "
		);
		void static destroy(TTF_Font *font);
};


class FontAtlas{
	public:
		Texture texture;
		unordered_map<char, Rect> data;

		FontAtlas(Texture &&_texture);

		void set_colour(const Colour colour);
		void draw_text(const string &text, const Vector &pos, const double scale=1);
		void draw_text(const string &text, const Vector &pos, const Colour &colour, const double scale=1);
};


#endif /* SUPERNOVA_FONT_H */