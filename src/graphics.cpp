#include "graphics.h"


using  namespace std;



// Classes
SpriteSheet::SpriteSheet(Renderer &renderer, const string &file, const int &column, const int &row): texture(Texture(renderer, file)) {
	tile_x = column; tile_y = row;
	total_tiles = tile_x*tile_y;
	src_rect = {0, 0, texture.w, texture.h};
	tile_w = src_rect.w/tile_x; tile_h = src_rect.h/tile_y;
}

void SpriteSheet::set_src_rect(const Rect &src_rect) {
	this->src_rect = src_rect;
	tile_w = src_rect.w/tile_x; tile_h = src_rect.h/tile_y;
}

void SpriteSheet::draw_sprite(const Rect &dst_rect, const int &column, const int &row) {
	texture.render(dst_rect, {src_rect.x + tile_w*column, src_rect.y + tile_h*row, tile_w, tile_h});
}


AnimatedSprite::AnimatedSprite(Renderer &renderer, const string &file, const int &column, const int &row, const double animation_speed, bool loop): SpriteSheet(renderer, file, column, row) {
	this->loop = loop;
	this->animation_speed = animation_speed;
}

bool AnimatedSprite::update(double dt) {
	// Returns true when the animation has just completed looping once
	animation_index += animation_speed*dt;
	if (animation_index > total_tiles) {
		animation_index -= total_tiles;
		return true;
	} else return false;
}

void AnimatedSprite::render(const Rect &dst_rect) {
	draw_sprite(dst_rect, fmod((int)animation_index, tile_x), floor((int)animation_index/tile_x));
}
