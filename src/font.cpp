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
}

int Font::wrap_alignment() {
	return TTF_GetFontWrappedAlign(font.get());
}

void Font::wrap_alignment(const int align) {
	TTF_SetFontWrappedAlign(font.get(), align);
}

Texture Font::create_text(Renderer &renderer, const string &text, const Colour &colour, const int &quality, const bool &wrap_text, const Uint32 &wrap_length, const Colour &background_colour) {
	SDL_Surface *text_surf;
	switch (quality) {
		case 0:
			if (wrap_text) 
				text_surf = TTF_RenderUTF8_Solid_Wrapped(
					font.get(),
					text.c_str(),
					{colour.r,colour.g, colour.b, colour.a},
					wrap_length
				);
			else
				text_surf = TTF_RenderUTF8_Solid(font.get(), text.c_str(), {colour.r, colour.g, colour.b, colour.a});
			break;
		case 1:
			if (wrap_text)
				text_surf = TTF_RenderUTF8_Shaded_Wrapped(
					font.get(),
					text.c_str(),
					{colour.r, colour.g, colour.b, colour.a},
					{background_colour.r, background_colour.g, background_colour.b, background_colour.a},
					wrap_length
				);
			else
				text_surf = TTF_RenderUTF8_Shaded(
					font.get(),
					text.c_str(),
					{colour.r, colour.g, colour.b, colour.a},
					{background_colour.r, background_colour.g, background_colour.b, background_colour.a}
				);
			break;
		case 2:
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
	}

	return Texture(renderer, Surface(text_surf));;
}

void Font::destroy(TTF_Font *font) {
	TTF_CloseFont(font);
	// SDL_Log("Font destroyed successfully![%i]", id);
}

FontAtlas Font::create_atlas(Renderer &renderer, const Colour colour, const int quality, const string chars) {
	TTF_SetFontKerning(font.get(), 0);
	FontAtlas atlas(create_text(renderer, chars, colour, quality));

	int w, h;
	for (int i = 0; i < static_cast<int>(chars.length()); i++) {
		char ch[] = { chars[i], '\0' };
		TTF_SizeUTF8(font.get(), ch, &w, &h);
		atlas.data[chars[i]] = {i*w, 0, w, atlas.texture.h};
	}

	TTF_SetFontKerning(font.get(), 1);
	return atlas;
}


FontAtlas::FontAtlas(Texture &&_texture): texture(std::move(_texture)) {}

void FontAtlas::set_colour(const Colour colour) {
	texture.set_colour_mod(colour);
}

void FontAtlas::draw_text(const string &text, const Vector &pos, const double scale) {
	int cx = pos.x;
	Rect *r;
	for (auto &ch: text) {
		r = &data[ch];
		texture.render({cx, static_cast<int>(pos.y), static_cast<int>(r->w*scale), static_cast<int>(r->h*scale)}, data[ch]);
		cx += r->w*scale;
	}
}

void FontAtlas::draw_text(const string &text, const Vector &pos, const Colour &colour, const double scale) {
	set_colour(colour);
	draw_text(text, pos, scale);
}
