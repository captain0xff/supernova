#include "font.h"

#include "logging.h"



// Globals
static int FONT_ID = 0;



// CLasses
Font::Font(const string &file, const int size):
	font(managed_ptr<TTF_Font>(TTF_OpenFont(file.c_str(), size), TTF_CloseFont)) {
	if (font.get() == nullptr)
		flog_error("Failed to load font! ({}): {}", file, SDL_GetError());
	else {
		id = FONT_ID;
		flog_info("Font loaded successfully![{}] ({})", id, file);
		FONT_ID++;
	}

	line_spacing = TTF_GetFontLineSkip(font.get());
}

int Font::wrap_alignment() {
	return TTF_GetFontWrapAlignment(font.get());
}

void Font::wrap_alignment(const TTF_HorizontalAlignment align) {
	TTF_SetFontWrapAlignment(font.get(), align);
}

int Font::kerning() {
	// GPOS kerning is not supported
	return TTF_GetFontKerning(font.get());
}

void Font::kerning(bool allowed) {
	// GPOS kerning is not supported
	TTF_SetFontKerning(font.get(), allowed);
}

int Font::get_glyph_kerning(const uint32_t prev_ch, const uint32_t ch) {
	// GPOS kerning is not supported
	int kerning;
	TTF_GetGlyphKerning(font.get(), prev_ch, ch, &kerning);

	return kerning;
}

void Font::add_fallback(Font &fallback) {
	if (!TTF_AddFallbackFont(font.get(), fallback.font.get())) {
		flog_error("Failed to add fallback font: {}", SDL_GetError());
	}
}
void Font::remove_fallback(Font &fallback) {
	TTF_RemoveFallbackFont(font.get(), fallback.font.get());
}
void Font::clear_fallbacks() {
	TTF_ClearFallbackFonts(font.get());
}

Surface Font::create_glyph(
	const uint32_t ch,
	const Colour colour,
	const int quality,
	const Colour &bg_colour
) {
	SDL_Surface *surf;

	switch (quality) {
		case FQ::SOLID:
			surf = TTF_RenderGlyph_Solid(font.get(), ch, colour);
			break;
		case FQ::SHADED:
			surf = TTF_RenderGlyph_Shaded(font.get(), ch, colour, bg_colour);
			break;
		case FQ::BLENDED:
			surf = TTF_RenderGlyph_Blended(font.get(), ch, colour);
			break;
		case FQ::LCD:
			surf = TTF_RenderGlyph_LCD(font.get(), ch, colour, bg_colour);
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
	const uint32_t wrap_length,
	const Colour &bg_colour
) {
	SDL_Surface *text_surf;

	switch (quality) {
		case FQ::SOLID:
			if (wrap_text) 
				text_surf = TTF_RenderText_Solid_Wrapped(
					font.get(),
					text.c_str(),
					text.size(),
					{colour.r,colour.g, colour.b, colour.a},
					wrap_length
				);
			else
				text_surf = TTF_RenderText_Solid(
					font.get(),
					text.c_str(),
					text.size(),
					{colour.r, colour.g, colour.b, colour.a}
				);
			break;
		case FQ::SHADED:
			if (wrap_text)
				text_surf = TTF_RenderText_Shaded_Wrapped(
					font.get(),
					text.c_str(),
					text.size(),
					{colour.r, colour.g, colour.b, colour.a},
					{bg_colour.r, bg_colour.g, bg_colour.b, bg_colour.a},
					wrap_length
				);
			else
				text_surf = TTF_RenderText_Shaded(
					font.get(),
					text.c_str(),
					text.size(),
					{colour.r, colour.g, colour.b, colour.a},
					{bg_colour.r, bg_colour.g, bg_colour.b, bg_colour.a}
				);
			break;
		case FQ::BLENDED:
			if (wrap_text)
				text_surf = TTF_RenderText_Blended_Wrapped(
					font.get(),
					text.c_str(),
					text.size(),
					{colour.r, colour.g, colour.b, colour.a},
					wrap_length
				);
			else
				text_surf = TTF_RenderText_Blended(
					font.get(),
					text.c_str(),
					text.size(),
					{colour.r,colour.g, colour.b, colour.a}
				);
			break;
		case FQ::LCD:
			if (wrap_text)
				text_surf = TTF_RenderText_LCD_Wrapped(
					font.get(),
					text.c_str(),
					text.size(),
					{colour.r, colour.g, colour.b, colour.a},
					{bg_colour.r, bg_colour.g, bg_colour.b, bg_colour.a},
					wrap_length
				);
			else
				text_surf = TTF_RenderText_LCD(
					font.get(),
					text.c_str(),
					text.size(),
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
	std::unordered_map<char, IRect> data;

	for (const char ch: chars) {
		TTF_GetStringSize(font.get(), &ch, 1, &w, &h);
		data[ch] = {dst, 0, w, h};
		dst += w;
	}

	Surface surf(IVector{dst, h});

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
	IRect *r;
	for (auto &ch: text) {
		r = &data[ch];
		texture.render(IRect{cx, pos.y, static_cast<int>(r->w*scale), static_cast<int>(r->h*scale)}, *r);
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
	IRect *r;
	for (auto &ch: text) {
		r = &data[ch];
		if (prev_ch != 0)
			kerning = font->get_glyph_kerning(prev_ch, ch);
		texture.render(
			IRect{
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


TextEngine::TextEngine(Renderer &renderer):
	renderer(renderer), engine(TTF_CreateRendererTextEngine(renderer.renderer.get()), TTF_DestroyRendererTextEngine)
{
	if (engine == nullptr) {
		flog_error("Failed to create text engine: {}", SDL_GetError());
	}
}


Text::Text(Font &font, TextEngine &text_engine, const string text, const Colour &colour):
	text_obj(TTF_CreateText(text_engine.engine.get(), font.font.get(), text.c_str(), text.size()), TTF_DestroyText)
{
	if (text_obj == nullptr) {
		flog_error("Failed to create text: {}", SDL_GetError());
	}
	TTF_SetTextColor(text_obj.get(), colour.r, colour.g, colour.b, colour.a);
}

void Text::font(Font &font) {
	TTF_SetTextFont(text_obj.get(), font.font.get());
}

void Text::engine(TextEngine &text_engine) {
	TTF_SetTextEngine(text_obj.get(), text_engine.engine.get());
}

void Text::colour(const Colour &colour) {
	TTF_SetTextColor(text_obj.get(), colour.r, colour.g, colour.b, colour.a);
}

void Text::text(const string &text) {
	TTF_SetTextString(text_obj.get(), text.c_str(), text.size());
}

void Text::wrap_width(const int width) {
	TTF_SetTextWrapWidth(text_obj.get(), width);
}

void Text::draw(const Vector &pos) {
	TTF_DrawRendererText(text_obj.get(), pos.x, pos.y);
}
