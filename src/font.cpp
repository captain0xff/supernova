#include "font.h"


using namespace std;



// Globals
int FONT_ID = 0;



// CLasses
Font::Font(const string &file, const int size):
	font(managed_ptr<TTF_Font>(TTF_OpenFont(file.c_str(), size), TTF_CloseFont)) {
	if (font.get() == nullptr)
		SDL_LogError(0, "Failed to load font! (%s): %s", file.c_str(), TTF_GetError());
	else {
		id = FONT_ID;
		SDL_Log("Font loaded successfully![%i] (%s)", id, file.c_str());
		FONT_ID++;
	}

	line_spacing = TTF_FontLineSkip(font.get());
}

int Font::wrap_alignment() {
	return TTF_GetFontWrappedAlign(font.get());
}

void Font::wrap_alignment(const int align) {
	TTF_SetFontWrappedAlign(font.get(), align);
}

int Font::kerning() {
	// GPOS kerning is not supported
	return TTF_GetFontKerning(font.get());
}

void Font::kerning(bool allowed) {
	// GPOS kerning is not supported
	TTF_SetFontKerning(font.get(), allowed);
}

int Font::get_glyph_kerning(const Uint32 prev_ch, const Uint32 ch) {
	// GPOS kerning is not supported
	return TTF_GetFontKerningSizeGlyphs32(font.get(), prev_ch, ch);
}

Surface Font::create_glyph(
	const Uint32 ch,
	const Colour colour,
	const int quality,
	const Colour &bg_colour
) {
	SDL_Surface *surf;

	switch (quality) {
		case FQ::SOLID:
			surf = TTF_RenderGlyph32_Solid(font.get(), ch, colour);
			break;
		case FQ::SHADED:
			surf = TTF_RenderGlyph32_Shaded(font.get(), ch, colour, bg_colour);
			break;
		case FQ::BLENDED:
			surf = TTF_RenderGlyph32_Blended(font.get(), ch, colour);
			break;
		case FQ::LCD:
			surf = TTF_RenderGlyph32_LCD(font.get(), ch, colour, bg_colour);
			break;
	}

	return Surface(surf);
}

Texture Font::create_text(
	Renderer &renderer,
	const string &text,
	const Colour &colour,
	const FONTQUALITY quality,
	const bool wrap_text,
	const Uint32 wrap_length,
	const Colour &bg_colour
) {
	SDL_Surface *text_surf;

	switch (quality) {
		case FQ::SOLID:
			if (wrap_text) 
				text_surf = TTF_RenderUTF8_Solid_Wrapped(
					font.get(),
					text.c_str(),
					{colour.r,colour.g, colour.b, colour.a},
					wrap_length
				);
			else
				text_surf = TTF_RenderUTF8_Solid(
					font.get(),
					text.c_str(),
					{colour.r, colour.g, colour.b, colour.a}
				);
			break;
		case FQ::SHADED:
			if (wrap_text)
				text_surf = TTF_RenderUTF8_Shaded_Wrapped(
					font.get(),
					text.c_str(),
					{colour.r, colour.g, colour.b, colour.a},
					{bg_colour.r, bg_colour.g, bg_colour.b, bg_colour.a},
					wrap_length
				);
			else
				text_surf = TTF_RenderUTF8_Shaded(
					font.get(),
					text.c_str(),
					{colour.r, colour.g, colour.b, colour.a},
					{bg_colour.r, bg_colour.g, bg_colour.b, bg_colour.a}
				);
			break;
		case FQ::BLENDED:
			if (wrap_text)
				text_surf = TTF_RenderUTF8_Blended_Wrapped(
					font.get(),
					text.c_str(),
					{colour.r, colour.g, colour.b, colour.a},
					wrap_length
				);
			else
				text_surf = TTF_RenderUTF8_Blended(
					font.get(),
					text.c_str(),
					{colour.r,colour.g, colour.b, colour.a}
				);
			break;
		case FQ::LCD:
			if (wrap_text)
				text_surf = TTF_RenderUTF8_LCD_Wrapped(
					font.get(),
					text.c_str(),
					{colour.r, colour.g, colour.b, colour.a},
					{bg_colour.r, bg_colour.g, bg_colour.b, bg_colour.a},
					wrap_length
				);
			else
				text_surf = TTF_RenderUTF8_LCD(
					font.get(),
					text.c_str(),
					{colour.r, colour.g, colour.b, colour.a},
					{bg_colour.r, bg_colour.g, bg_colour.b, bg_colour.a}
				);
			break;
	}

	return Texture(renderer, Surface(text_surf));
}

FontAtlas Font::create_atlas(
	Renderer &renderer,
	const Colour &colour,
	const FONTQUALITY quality,
	const Colour &bg_colour,
	const string chars
) {
	int dst = 0;
	int w, h;
	unordered_map<char, Rect> data;

	for (const char ch: chars) {
		char chr[] = {ch, '\0'};
		TTF_SizeUTF8(font.get(), chr, &w, &h);
		data[ch] = {dst, 0, w, h};
		dst += w;
	}

	Surface surf(dst, h);

	for (const char ch: chars) {
		Surface glyph = create_glyph(ch, colour, quality, bg_colour);
		glyph.blit(surf, data[ch]);
	}

	FontAtlas atlas(this, Texture(renderer, std::move(surf)));
	atlas.data = data;

	return atlas;
}


FontAtlas::FontAtlas(Font *font, Texture &&_texture): texture(std::move(_texture)) {
	this->font = font;
}

void FontAtlas::set_colour(const Colour colour) {
	texture.set_colour_mod(colour);
}

void FontAtlas::draw_text(const string &text, const IVector &pos, const double scale) {
	int cx = pos.x;
	Rect *r;
	for (auto &ch: text) {
		r = &data[ch];
		texture.render({cx, pos.y, static_cast<int>(r->w*scale), static_cast<int>(r->h*scale)}, *r);
		cx += r->w*scale;
	}
}

void FontAtlas::draw_text(const string &text, const IVector &pos, const Colour &colour, const double scale) {
	set_colour(colour);
	draw_text(text, pos, scale);
}

void FontAtlas::draw_text_with_kerning(const string &text, const IVector &pos, const double scale) {
	// This function will only work if the font has kerning tables
	// GPOS kerning is not supported
	int cx = pos.x;
	int kerning = 0;
	char prev_ch = 0;
	Rect *r;
	for (auto &ch: text) {
		r = &data[ch];
		if (prev_ch != 0)
			kerning = font->get_glyph_kerning(prev_ch, ch);
		texture.render(
			{
				static_cast<int>(cx + kerning*scale),
				pos.y,
				static_cast<int>(r->w*scale),
				static_cast<int>(r->h*scale)
			},
			*r
		);
		cx += r->w*scale + kerning*scale;
		prev_ch = ch;
	}
}

void FontAtlas::draw_text_with_kerning(const string &text, const IVector &pos, const Colour &colour, const double scale) {
	// This function will only work if the font has kerning tables
	// GPOS kerning is not supported
	set_colour(colour);
	draw_text_with_kerning(text, pos, scale);
}
