#ifndef SUPERNOVA_FONT_H
#define SUPERNOVA_FONT_H

#ifndef TTF_ENABLED
#define TTF_ENABLED
#endif /* TTF_ENABLED */


#include <SDL3_ttf/SDL_ttf.h>

#include "core.h"
#include "enums.h"
#include "constants.h"



// Forward Declarations
class FontAtlas;



// Classes
class Font {
public:
	int id;
	int line_spacing;
	managed_ptr<TTF_Font> font;

	Font(const string &file, const int size);

	int wrap_alignment();
	void wrap_alignment(const TTF_HorizontalAlignment align);
	// These functions will only work if the font has kerning tables
	// GPOS kerning is not supported
	int kerning();
	void kerning(bool allowed);
	int get_glyph_kerning(const uint32_t prev_ch, const uint32_t ch);
	void add_fallback(Font &fallback);
	void remove_fallback(Font &fallback);
	void clear_fallbacks();
	Surface create_glyph(
		const uint32_t ch,
		const Colour colour,
		const int quality=0,
		const Colour &bg_colour={0, 0, 0, 0}
	);
	Texture create_text(
		Renderer &renderer,
		const string &text,
		const Colour &colour,
		const FONTQUALITY quality=FQ::SOLID,
		const bool wrap_text=true,
		const uint32_t wrap_length=0,
		const Colour &bg_colour={0, 0, 0, 0}
	);
	FontAtlas create_atlas(
		Renderer &renderer,
		const Colour &colour={255, 255, 255, 255},
		const FONTQUALITY quality=FQ::SOLID,
		const Colour &bg_colour={0, 0, 0, 0},
		const string chars="abcdefghijklmnopqrstuvwxyz\
							ABCDEFGHIJKLMNOPQRSTUVWXYZ\
							1234567890.:,;'\"(!?)+-*/= "
	);
};


class FontAtlas {
public:
	Font *font;
	Texture texture;
	std::unordered_map<char, IRect> data;

	FontAtlas(Font *font, Texture &&_texture);

	void set_colour(const Colour colour);
	void draw_text(
		const string &text,
		const IVector &pos,
		const double scale=1
	);
	void draw_text(
		const string &text,
		const IVector &pos,
		const Colour &colour,
		const double scale=1
	);
	// These functions will only work if the font has kerning tables
	// GPOS kerning is not supported
	void draw_text_with_kerning(
		const string &text,
		const IVector &pos,
		const double scale=1
	);
	void draw_text_with_kerning(
		const string &text,
		const IVector &pos,
		const Colour &colour,
		const double scale=1
	);
};


class TextEngine {
public:
	Renderer &renderer;
	managed_ptr<TTF_TextEngine> engine;

	TextEngine(Renderer &renderer);
};


class Text {
public:
	managed_ptr<TTF_Text> text_obj;

	Text(Font &font, TextEngine &text_engine, const string text, const Colour &colour=WHITE);

	// TODO: Add get variants of these functions
	void font(Font &font);
	void engine(TextEngine &text_engine);
	void colour(const Colour &colour);
	// Recreates the whole TTF_Text object internally
	void text(const string &text);
	// If width is 0 then only wraps in newlines
	void wrap_width(const int width=0);
	void draw(const Vector &pos);
};


#endif /* SUPERNOVA_FONT_H */