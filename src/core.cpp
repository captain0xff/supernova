#include "core.h"

#include <cassert>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <cstdlib>

#ifdef IMAGE_ENABLED
#include <SDL3_image/SDL_image.h>
#endif /* IMAGE_ENABLED */
#ifdef MIXER_ENABLED
#include <SDL3_mixer/SDL_mixer.h>
#endif /* MIXER_ENABLED */
#ifdef TTF_ENABLED
#include <SDL3_ttf/SDL_ttf.h>
#endif /* TTF_ENABLED */
#ifdef NET_ENABLED
#include <SDL3_net/SDL_net.h>
#endif /* NET_ENABLED */

#include "constants.h"
#include "logging.h"



// Globals
static int SURF_ID = 0;
static int TEX_ID = 0;



// Helper functions
double radians(const double angle) {
	// Degree to radians
	return angle * (PI / 180);
}

double degrees(const double angle) {
	return angle * (180 / PI);
}


int randint(const int end) {
	// Generates a random integer b/w 0 to end (0 included and end excluded).
	return rand()%end;
}

int randint(const int start, const int end) {
	// Generates a random integer b/w start to end (start included and end excluded).
	return start + rand()%(end - start);
}

void image_function_not_implemented(const string &str) {
	flog_error("Engine was not built with SDL_image support! {} not available.", str);
	assert(0);
}



// General functions
#ifdef ___ANDROID__
void trigger_back_button() {
	SDL_AndroidBackButton();
}

void* get_activity() {
	void *activity;
	if ((activity = SDL_AndroidGetActivity()) == NULL) {
		flog_error("Failed to get the android activity: {}", SDL_GetError());
	}

	return activity;
}

string get_external_storage_path() {
	return SDL_AndroidGetExternalStoragePath();
}

int get_external_storage_state() {
	uint32_t state;
	if (SDL_AndroidGetExternalStorageState(&state) < 0) {
		flog_error("Failed to get external storage state: {}", SDL_GetError());
	}
	return state;
}

string get_internal_storage_path() {
	return SDL_AndroidGetInternalStoragePath();
}

void* get_jni_env() {
	void *jni;
	if ((jni = SDL_AndroidGetJNIEnv()) == 0) {
		flog_error("Failed to get JNIEnv: {}", SDL_GetError());
	}

	return jni;
}

// bool get_permission(const string permission) {
// 	// Returns if the permission was granted or not
// 	return SDL_AndroidRequestPermission(permission.c_str());
// }

void show_toast(const string message, const bool duration, const int gravity, const int offsetx, const int offsety) {
	// Set offset only when gravity is non-zero
	SDL_AndroidShowToast(message.c_str(), duration, gravity, offsetx, offsety);
}

#endif /* __ANDROID__ */



// Structs
std::ostream& operator<<(std::ostream &os, Colour const &colour) {
	std::cout << colour.to_str();
		return os;
}

Colour operator*(const Colour &colour, const float val) {
	return {
		static_cast<uint8_t>(colour.r*val),
		static_cast<uint8_t>(colour.g*val),
		static_cast<uint8_t>(colour.b*val),
		static_cast<uint8_t>(colour.a*val)
	};
}

Colour operator/(const Colour &colour, const float val) {
	return {
		static_cast<uint8_t>(colour.r/val),
		static_cast<uint8_t>(colour.g/val),
		static_cast<uint8_t>(colour.b/val),
		static_cast<uint8_t>(colour.a/val)
	};
}

void operator*=(Colour &colour, const float val) {
	colour.r *= val;
	colour.g *= val;
	colour.b *= val;
	colour.a *= val;
}

void operator/=(Colour &colour, const float val) {
	colour.r /= val;
	colour.g /= val;
	colour.b /= val;
	colour.a /= val;
}

Colour::operator FColour() const{
	return {
		static_cast<float>(r/255.0f),
		static_cast<float>(g/255.0f),
		static_cast<float>(b/255.0f),
		static_cast<float>(a/255.0f)
	};
}

Colour::operator SDL_Color() const {
	return {r, g, b, a};
}

const string Colour::to_str() const {
	return "Colour(" + std::to_string(r) + ", " + std::to_string(g) + std::to_string(b) + std::to_string(a) + ")";
}

Colour Colour::modulate(const float mod_r, const float mod_g, const float mod_b, const float mod_a) const {
	return {
		static_cast<uint8_t>(r*mod_r),
		static_cast<uint8_t>(g*mod_g),
		static_cast<uint8_t>(b*mod_b),
		static_cast<uint8_t>(a*mod_a)
	};
}


std::ostream& operator<<(std::ostream &os, const FColour &fcolour) {
	std::cout << fcolour.to_str();
		return os;
}

FColour operator*(const FColour &fcolour, const float val) {
	return {
		fcolour.r*val,
		fcolour.g*val,
		fcolour.b*val,
		fcolour.a*val
	};
}

FColour operator/(const FColour &fcolour, const float val) {
	return {
		fcolour.r/val,
		fcolour.g/val,
		fcolour.b/val,
		fcolour.a/val
	};
}

void operator*=(FColour &fcolour, const float val) {
	fcolour.r *= val;
	fcolour.g *= val;
	fcolour.b *= val;
	fcolour.a *= val;
}

void operator/=(FColour &fcolour, const float val) {
	fcolour.r /= val;
	fcolour.g /= val;
	fcolour.b /= val;
	fcolour.a /= val;
}

FColour::operator Colour() const{
	return {
		static_cast<uint8_t>(r*255),
		static_cast<uint8_t>(g*255),
		static_cast<uint8_t>(b*255),
		static_cast<uint8_t>(a*255)
	};
}

FColour::operator SDL_FColor() const {
	return {r, g, b, a};
}

const string FColour::to_str() const {
	return "FColour(" + std::to_string(r) + ", " + std::to_string(g) + std::to_string(b) + std::to_string(a) + ")";
}

FColour FColour::modulate(const float mod_r, const float mod_g, const float mod_b, const float mod_a) const {
	return {
		r*mod_r,
		g*mod_g,
		b*mod_b,
		a*mod_a
	};
}


std::ostream& operator<<(std::ostream &os, IVector const &ivector) {
	std::cout << ivector.to_str();
		return os;
}

IVector operator+(const IVector &ivec1, const IVector &ivec2) {
	return {ivec1.x + ivec2.x, ivec1.y + ivec2.y};
}

IVector operator-(const IVector &ivec1, const IVector &ivec2) {
	return {ivec1.x - ivec2.x, ivec1.y - ivec2.y};
}

IVector operator*(const IVector &ivec, const float &val) {
	return {static_cast<int>(ivec.x*val), static_cast<int>(ivec.y*val)};
}

IVector operator/(const IVector &ivec, const float &val) {
	return {static_cast<int>(ivec.x/val), static_cast<int>(ivec.y/val)};
}

void operator+=(IVector &ivec1, const IVector &ivec2) {
	ivec1.x += ivec2.x;
	ivec1.y += ivec2.y;
}

void operator-=(IVector &ivec1, const IVector &ivec2) {
	ivec1.x -= ivec2.x;
	ivec1.y -= ivec2.y;
}

void operator*=(IVector &ivec1, const IVector &ivec2) {
	ivec1.x *= ivec2.x;
	ivec1.y *= ivec2.y;
}

void operator/=(IVector &ivec1, const IVector &ivec2) {
	ivec1.x /= ivec2.x;
	ivec1.y /= ivec2.y;
}

IVector::operator Vector() const {
	return {static_cast<float>(x), static_cast<float>(y)};
}

IVector::operator SDL_Point() const {
	return {x, y};
}

IVector::operator SDL_FPoint() const {
	return {static_cast<float>(x), static_cast<float>(y)};
}

const string IVector::to_str() const {
	return "IVector(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}


std::ostream& operator<<(std::ostream &os, const Vector &vector) {
	std::cout << vector.to_str();
	return os;
}

Vector operator+(const Vector &vec1, const Vector &vec2) {
	return {vec1.x + vec2.x, vec1.y + vec2.y};
}

Vector operator-(const Vector &vec1, const Vector &vec2) {
	return {vec1.x - vec2.x, vec1.y - vec2.y};
}

Vector operator*(const Vector &vec, const float &val) {
	return {vec.x*val, vec.y*val};
}

Vector operator/(const Vector &vec, const float &val) {
	return {vec.x/val, vec.y/val};
}

void operator+=(Vector &vec1, const Vector &vec2) {
	vec1.x += vec2.x;
	vec1.y += vec2.y;
}

void operator-=(Vector &vec1, const Vector &vec2) {
	vec1.x -= vec2.x;
	vec1.y -= vec2.y;
}

void operator*=(Vector &vec1, const Vector &vec2) {
	vec1.x *= vec2.x;
	vec1.y *= vec2.y;
}

void operator/=(Vector &vec1, const Vector &vec2) {
	vec1.x /= vec2.x;
	vec1.y /= vec2.y;
}

Vector::operator IVector() const {
	return {static_cast<int>(x), static_cast<int>(y)};
}

Vector::operator SDL_Point() const {
	return {static_cast<int>(x), static_cast<int>(y)};
}

Vector::operator SDL_FPoint() const {
	return {x, y};
}

const string Vector::to_str() const {
	return "Vector(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

float Vector::magnitude_squared() const {
	return pow(x, 2) + pow(y, 2);
}

float Vector::magnitude() const {
	return pow(magnitude_squared(), 0.5);
}

Vector Vector::normalize() const {
	return {x / magnitude(), y / magnitude()};
}

void Vector::normalize_ip() {
	x = x / magnitude();
	y = y / magnitude();
}

Vector Vector::rotate_rad(const float &angle) const {
	return {
		static_cast<float>(x*cos(angle) - y*sin(angle)),
		static_cast<float>(x*sin(angle) + y*cos(angle))
	};
}

void Vector::rotate_rad_ip(const float &angle) {
	float temp = x;
	x = x*cos(angle) - y*sin(angle);
	y = temp*sin(angle) + y*cos(angle);
}

Vector Vector::rotate(const float &angle) const {
	return rotate_rad(radians(angle));
}

void Vector::rotate_ip(const float &angle) {
	rotate_rad_ip(radians(angle));
}

float Vector::distance_to_squared(const Vector &vec) const {
	return pow(vec.x - x, 2) + pow(vec.y - y, 2);
}

float Vector::distance_to(const Vector &vec) const {
	return sqrt(distance_to_squared(vec));
}

float Vector::angle_rad() const {
	float temp = atan2(y, x);
	return (temp > 0) ? temp : 2*PI + temp;
}

float Vector::angle() const {
	return degrees(angle_rad());
}

Vector Vector::clamp(const Rect &rect) const {
	Vector vec = {x, y};

	if (rect.left() > x)
		vec.x = rect.left();
	else if (rect.right() < x)
		vec.x = rect.right();

	if (rect.top() > y)
		vec.y = rect.top();
	else if (rect.bottom() < y)
		vec.y = rect.bottom();

	return vec;
}

Vector Vector::clamp(const Circle &circle) const {
	Vector vec = {x, y};

	if (this->distance_to(circle.center()) > circle.r)
		vec = circle.center() + (vec - circle.center()).normalize()*circle.r;

	return vec;
}

void Vector::clamp_ip(const Rect &rect) {
	if (rect.left() > x)
		x = rect.left();
	else if (rect.right() < x)
		x = rect.right();

	if (rect.top() > y)
		y = rect.top();
	else if (rect.bottom() < y)
		y = rect.bottom();
}

void Vector::clamp_ip(const Circle &circle) {
	float temp = this->distance_to(circle.center());

	if (temp > circle.radius()){
		x = circle.x + (x - circle.x)*circle.r/temp;
		y = circle.y + (y - circle.y)*circle.r/temp;
	}
}


IRect::IRect(int x, int y, int w, int h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

IRect::IRect(const IVector &pos, const IVector &size) {
	x = pos.x;
	y = pos.y;
	w = size.x;
	h = size.y;
}

std::ostream& operator<<(std::ostream &os, IRect const &IRect) {
	std::cout << IRect.to_str();
	return os;
}

IRect::operator Rect() const {
	return {static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h)};
}

IRect::operator SDL_Rect() const {
	return {x, y, w, h};
}

const string IRect::to_str() const {
	return "Rect(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(w) + ", " + std::to_string(h) + ")";
}


Rect::Rect(float x, float y, float w, float h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

Rect::Rect(const Vector &pos, const Vector &size) {
	x = pos.x;
	y = pos.y;
	w = size.x;
	h = size.y;
}

std::ostream& operator<<(std::ostream &os, Rect const &rect) {
	std::cout << rect.to_str();
	return os;
}

Rect::operator SDL_FRect() const {
	return {x, y, w, h};
}

const string Rect::to_str() const {
	return "Rect(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(w) + ", " + std::to_string(h) + ")";
}

Vector Rect::size() const {
	return {w, h};
}

void Rect::size(const Vector &vec) {
	w = vec.x;
	h = vec.y;
}

void Rect::scale(const Vector &vec) {
	w *= vec.x;
	h *= vec.y;
}

void Rect::scale(const float val) {
	w *= val;
	h *= val;
}

float Rect::half_width() const {
	return w / 2;
}

float Rect::half_height() const {
	return h / 2;
}

Vector Rect::half_size() const {
	return {half_width(), half_width()};
}

float Rect::top() const {
	return y;
}

void Rect::top(const float &val) {
	y = val;
}

float Rect::bottom() const {
	return y + h;
}

void Rect::bottom(const float &val) {
	y = val - h;
}

float Rect::left() const {
	return x;
}

void Rect::left(const float &val) {
	x = val;
}

float Rect::right() const {
	return x + w;
}

void Rect::right(const float &val) {
	x = val - w;
}

float Rect::centerx() const {
	return x + half_width();
}

void Rect::centerx(const float &val) {
	x = val - half_width();
}

float Rect::centery() const {
	return y + half_height();
}

void Rect::centery(const float &val) {
	y = val - half_height();
}

Vector Rect::topleft() const {
	return {left(), top()};
}

void Rect::topleft(const Vector &vec) {
	top(vec.y);
	left(vec.x);
}

Vector Rect::topright() const {
	return {right(), top()};
}

void Rect::topright(const Vector &vec) {
	top(vec.y);
	right(vec.x);
}

Vector Rect::bottomleft() const {
	return {left(), bottom()};
}

void Rect::bottomleft(const Vector &vec) {
	bottom(vec.y);
	left(vec.x);
}

Vector Rect::bottomright() const {
	return {right(), bottom()};
}

void Rect::bottomright(const Vector &vec) {
	bottom(vec.y);
	right(vec.x);
}

Vector Rect::center() const {
	return {centerx(), centery()};
}

void Rect::center(const Vector &vec) {
	centery(vec.y);
	centerx(vec.x);
}

Vector Rect::midtop() const {
	return {centerx(), top()};
}

void Rect::midtop(const Vector &vec) {
	centerx(vec.x);
	top(vec.y);
}

Vector Rect::midbottom() const {
	return {centerx(), bottom()};
}

void Rect::midbottom(const Vector &vec) {
	centerx(vec.x);
	bottom(vec.y);
}

Vector Rect::midleft() const {
	return {left(), centery()};
}

void Rect::midleft(const Vector &vec) {
	left(vec.x);
	centery(vec.y);
}

Vector Rect::midright() const {
	return {right(), centery()};
}

void Rect::midright(const Vector &vec) {
	right(vec.x);
	centery(vec.y);
}

bool Rect::collide_point(const Vector &vec) const {
	if (((left() <= vec.x) && (vec.x <= right())) && ((top() <= vec.y) && (vec.y <= bottom())))
		return true;
	return false;
}

bool Rect::collide_rect(const Rect &rect) const {
	if ((left() < rect.x + rect.w) && (right() > rect.x) && (top() < rect.y + rect.h) && (bottom() > rect.y))
		return true;
	return false;
}

Rect Rect::clamp(const Rect &rect) const {
	// Clamps the rect within the rect passed
	Rect new_rect = {x, y, w, h};
	if (left() < rect.x)
		new_rect.left(rect.x);
	else if (right() > (rect.x + rect.w))
		new_rect.right(rect.x + rect.w);
	if (top() < rect.y)
		new_rect.top(rect.y);
	else if (bottom() > (rect.y + rect.h))
		new_rect.bottom(rect.y + rect.h);
	return new_rect;
}

bool Rect::clamp_ip(const Rect &rect) {
	// Returns if the rect is clamped or not
	bool is_changed = false;

	if (left() < rect.x) {
		is_changed = true;
		left(rect.x);
	} else if (right() > (rect.x + rect.w)) {
		is_changed = true;
		right(rect.x + rect.w);
	}

	if (top() < rect.y) {
		is_changed = true;
		top(rect.y);
	} else if (bottom() > (rect.y + rect.h)) {
		is_changed = true;
		bottom(rect.y + rect.h);
	}

	return is_changed;
}

Rect Rect::move(const Vector &vec) const {
	return {x + vec.x, y + vec.y, w, h};
}

void Rect::move_ip(const Vector &vec) {
	x += vec.x;
	y += vec.y;
}


Circle::Circle(float x, float y, float r) {
	this->x = x;
	this->y = y;
	this->r = r;
}

Circle::Circle(const Vector &vec, const float radius) {
	x = vec.x;
	y = vec.y;
	r = radius;
}

std::ostream& operator<<(std::ostream &os, const Circle &circle) {
	std::cout << circle.to_str();
	return os;
}

const string Circle::to_str() const {
	return "Circle(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(r) + ")";
}

float Circle::radius() const {
	return r;
}

void Circle::radius(const float radius) {
	r = radius;
}

Vector Circle::center() const {
	return {x, y};
}

void Circle::center(const Vector &vec) {
	x = vec.x;
	y = vec.y;
}

bool Circle::collide_point(const Vector &vec) const {
	if (center().distance_to_squared(vec) > pow(r, 2))
		return false;
	else
		return true;
}

bool Circle::collide_circle(const Circle &circle) const {
	if (circle.center().distance_to_squared(center()) > pow(circle.r + r, 2))
		return false;
	else
		return true;
}

Circle Circle::clamp(const Circle &circle) const {
	Circle new_circle = {x, y, r};
	Vector center_diff = center() - circle.center();

	if (center_diff.magnitude() > circle.r - r) {
		new_circle.center(center_diff.normalize()*(circle.r - r) + circle.center());
	}

	return new_circle;
}

bool Circle::clamp_ip(const Circle &circle) {
	// Returns if the circle is clamped or not
	Vector center_diff = center() - circle.center();

	if (center_diff.magnitude() > circle.r - r) {
		center(center_diff.normalize()*(circle.r - r) + circle.center());
		return true;
	} else
		return false;
}

Circle Circle::move(const Vector &vec) const {
	return {x + vec.x, y + vec.y, r};
}

void Circle::move_ip(const Vector &vec) {
	x += vec.x;
	y += vec.y;
}


EngineArgs::EngineArgs():
	sdl_init_flags(SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_AUDIO)
#ifdef IMAGE_ENABLED
	, img_init_flags(IMG_INIT_PNG|IMG_INIT_JPG)
#endif /* IMAGE_ENABLED */
#ifdef MIXER_ENABLED
	, mix_init_flags(MIX_INIT_MP3|MIX_INIT_OGG)
#endif /* MIXER_ENABLED */
{}



// Classes
Engine::Engine(const EngineArgs &args) {
	if (SDL_Init(args.sdl_init_flags) < 0)
		flog_error("Failed to initialize SDL: {}", SDL_GetError());
#ifdef IMAGE_ENABLED
	if (IMG_Init(args.img_init_flags) != args.img_init_flags)
		flog_error("Failed to initialize SDL_image: {}", IMG_GetError());
#endif /* IMAGE_ENABLED */
#ifdef MIXER_ENABLED
	if (Mix_Init(args.mix_init_flags) != args.mix_init_flags)
		flog_error("Failed to initialize SDL_mixer: {}", Mix_GetError());
#endif /* MIXER_ENABLED */
#ifdef TTF_ENABLED
	if (TTF_Init() < 0)
		flog_error("Failed to initialize SDL_ttf: {}", TTF_GetError());
#endif /* TTF_ENABLED */
#ifdef NET_ENABLED
	if (SDLNet_Init() < 0)
		flog_error("Failed to initialize SDL_net: {}", SDL_GetError());
#endif /* TTF_ENABLED */
	srand((unsigned) time(NULL)); // Create a seed for random number generation
	flog_info("Engine started!");
}

Engine::~Engine() {
#ifdef IMAGE_ENABLED
	IMG_Quit();
#endif /* IMAGE_ENABLED */
#ifdef MIX_ENABLED
	Mix_Quit();
#endif /* MIX_ENABLED */
#ifdef TTF_ENABLED
	TTF_Quit();
#endif /* TTF_ENABLED */
#ifdef NET_ENABLED
	SDLNet_Quit();
#endif /* NET_ENABLED */
	SDL_Quit();
	flog_info("Engine stopped!");
}


uint64_t Clock::get_ticks() {
	return SDL_GetTicks();
}

double Clock::tick(double target_fps) {
	// The parameter target_fps should be 0 for unclamped fps
	if (target_fps) {
		target_ft = 1000/target_fps;
		raw_time = SDL_GetTicks() - last_tick;
		delay = target_ft - raw_time;

		if (delay > 0)
			SDL_Delay((uint32_t)delay);
	}

	current_time = SDL_GetTicks();
	frame_time = current_time - last_tick;
	last_tick = current_time;

	if (!target_fps)
		raw_time = frame_time;

	return (double)frame_time/1000;
}

double Clock::get_fps() {
	return 1000/(double)frame_time;
}

double Clock::timeit() {
	// Returns the time between two of its successive calls in ms
	current_time = SDL_GetPerformanceCounter();
	double time_diff = ((current_time - timeit_tick)/(double)SDL_GetPerformanceFrequency())*1000;
	timeit_tick = current_time;
	return time_diff;
}


Timer::Timer(double _time) {
	// Time should be in seconds
	time = _time;
}

bool Timer::update(double dt, bool _reset) {
	// Returns true once after the set time is over and gets reset
	counter += dt;
	if (counter >= time) {
		if (_reset)
			reset();
		return true;
	}
	return false;
}

void Timer::reset() {
	// Sets the counter to 0
	counter = 0;
}

double Timer::time_left() {
	return time - counter;
}


IO::IO(const string &file, const string access_mode) {
	io = SDL_IOFromFile(file.c_str(), access_mode.c_str());

	if (io == NULL)
		flog_error("Failed to load file! ({}): {}", file, SDL_GetError());
	else
		IS_LOADED = true;
}

IO::~IO() {
	close();
}

int64_t IO::get_file_size() {
	const Sint64 file_pos = tell();
	const Sint64 file_size = seek(0, SDL_IO_SEEK_END);
	seek(file_pos, SDL_IO_SEEK_SET);

	return file_size;
}

int IO::read(void *ptr, const int max) {
	// The parameter takes the maximum number of bytes to read
	// Returns the number of objects read or -1 on error
	if (IS_LOADED)
		return SDL_ReadIO(io, ptr, max);
	flog_warn("Failed to read: file not loaded successfully!");
	return -1;
}

string IO::read() {
	// Reads the whole file at once to a string
	int size;
	Sint64 file_size = get_file_size();
	std::vector<char> buffer(file_size + 1);

	if ((size = read(buffer.data(), file_size)) == file_size) {
		buffer[size] = '\0';
		return string(buffer.data());
	}
	return NULL;
}

string IO::read(const int max) {
	// Reads the next max number of chars from the file to a string
	int size;
	std::vector<char> buffer(max + 1);

	if ((size = read(buffer.data(), max)) >= 0) {
		buffer[size] = '\0';
		return string(buffer.data());
	}
	return NULL;
}

void IO::write(const void *ptr, const size_t num) {
	// The num parameter takes the number of objects to write
	// Returns the numer of objects written
	if (!IS_LOADED)
		flog_warn("Failed to write: file not loaded successfully!");
	else if (SDL_WriteIO(io, ptr, num) < num)
		flog_warn("Failed to write all the objects: {}", SDL_GetError());
}

void IO::write(const string &data) {
	write(data.c_str(), data.length());
}

int64_t IO::tell() {
	if (IS_LOADED)
		return SDL_TellIO(io);
	flog_warn("Failed to tell: file not loaded successfully!");
	return -1;
}

int64_t IO::seek(int64_t offset, SDL_IOWhence whence) {
	if (IS_LOADED)
		return SDL_SeekIO(io, offset, whence);
	flog_warn("Failed to seek: file not loaded successfully!");
	return -1;
}

void IO::close() {
	if (io != nullptr) {
		SDL_CloseIO(io);
		io = nullptr;
		IS_LOADED = false;
	}
}


Window::Window(const string &title, const IVector &size, const uint32_t flags):
	window(managed_ptr<SDL_Window>(SDL_CreateWindow(title.c_str(), size.x,size.y, flags), destroy)) {
	if (window.get() == NULL)
		flog_error("Failed to create window: {}", SDL_GetError());
	else
		flog_info("Window created successfully!");
}

void Window::wrap_mouse(const Vector &wrap_pos) {
	SDL_WarpMouseInWindow(window.get(), static_cast<int>(wrap_pos.x), static_cast<int>(wrap_pos.y));
}

Surface Window::get_window_surface() {
	return Surface(SDL_GetWindowSurface(window.get()));
}

void Window::gl_swap() {
	SDL_GL_SwapWindow(window.get());
}

void Window::start_text_input() {
	SDL_StartTextInput(window.get());
}

void Window::destroy(SDL_Window *window) {
	SDL_DestroyWindow(window);
	flog_info("Window closed successfully!");
}


Renderer::Renderer(Window &window, const string &driver):
		renderer(managed_ptr<SDL_Renderer>((driver == "")? SDL_CreateRenderer(window.window.get(), NULL) : SDL_CreateRenderer(window.window.get(), driver.c_str()),destroy)) {
	if (renderer.get() == NULL)
		flog_error("Failed to create renderer: {}", SDL_GetError());
	else
		flog_info("Renderer created successfully!");
}

void Renderer::set_colour(const Colour &colour) {
	SDL_SetRenderDrawColor(renderer.get(), colour.r, colour.g, colour.b, colour.a);
}

void Renderer::clear(const Colour &colour) {
	set_colour(colour);
	SDL_RenderClear(renderer.get());
}

void Renderer::present() {
	SDL_RenderPresent(renderer.get());
}

void Renderer::flush() {
	SDL_FlushRenderer(renderer.get());
}

void Renderer::set_blend_mode(const SDL_BlendMode blendmode) {
	SDL_SetRenderDrawBlendMode(renderer.get(), blendmode);
}

void Renderer::set_target() {
	SDL_SetRenderTarget(renderer.get(), NULL);
}

void Renderer::set_target(Texture &tex) {
	SDL_SetRenderTarget(renderer.get(), tex.texture.get());
}

void Renderer::set_logical_presentation(const IVector &size, const SDL_RendererLogicalPresentation mode, SDL_ScaleMode scale_mode) {
	SDL_SetRenderLogicalPresentation(renderer.get(), size.x, size.y, mode, scale_mode);
}

IVector Renderer::get_output_size() {
	int w, h;
	SDL_GetCurrentRenderOutputSize(renderer.get(), &w, &h);
	return {w, h};
}

string Renderer::get_driver_name() {
	return string(SDL_GetRendererName(renderer.get()));
}

void Renderer::draw_point_raw(const Vector &point_pos) {
	SDL_RenderPoint(renderer.get(), point_pos.x, point_pos.y);
}

void Renderer::draw_point(const Vector &point_pos, const Colour &colour) {
	set_colour(colour);
	draw_point_raw(point_pos);
}

void Renderer::draw_line_raw(const Vector &v1, const Vector &v2) {
	SDL_RenderLine(renderer.get(), v1.x, v1.y, v2.x, v2.y);
}

void Renderer::draw_line(const Vector &v1, const Vector &v2, const Colour &colour) {
	set_colour(colour);
	draw_line_raw(v1, v2);
}

void Renderer::draw_line(const Vector &v1, const Vector &v2, const Colour &colour, const float width) {
	float slope = atan2(v1.x - v2.x, v2.y - v1.y);
	float x = (width/2)*cos(slope);
	float y = (width/2)*sin(slope);

	constexpr Vector tex_coord = {0, 0}; // The coords are not used

	SDL_Vertex vertices[4] = {
		{{v1.x + x, v1.y + y}, (FColour)colour, tex_coord},
		{{v1.x - x, v1.y - y}, (FColour)colour, tex_coord},
		{{v2.x - x, v2.y - y}, (FColour)colour, tex_coord},
		{{v2.x + x, v2.y + y}, (FColour)colour, tex_coord},
	};
	int indices[6] = {0, 1, 2, 0, 2, 3};

	SDL_RenderGeometry(renderer.get(), NULL, vertices, 4, indices, 6);
}

void Renderer::draw_rect_raw(const Rect &rect, float width) {
	if (width == 0) {
		SDL_FRect r = rect;
		SDL_RenderFillRect(renderer.get(), &r);
	} else if (width == 1) {
		SDL_FRect r = rect;
		SDL_RenderRect(renderer.get(), &r);
	} else {
		SDL_FRect r1 = {rect.x - width, rect.y - width, rect.w + 2*width, width};
		SDL_FRect r2 = {rect.x - width, rect.y + rect.h, rect.w + 2*width, width};
		SDL_FRect r3 = {rect.x - width, rect.y, width, rect.h};
		SDL_FRect r4 = {rect.x + rect.w, rect.y, width, rect.h};
		SDL_RenderFillRect(renderer.get(), &r1);
		SDL_RenderFillRect(renderer.get(), &r2);
		SDL_RenderFillRect(renderer.get(), &r3);
		SDL_RenderFillRect(renderer.get(), &r4);
	}
}

void Renderer::draw_rect(const Rect &rect, const Colour &colour, float width) {
	set_colour(colour);
	draw_rect_raw(rect, width);
}

void Renderer::draw_circle(const Circle &circle, const Colour &colour, const bool filled) {
	if (filled) {
		constexpr int tris = 225; // Amount of triangles
		float mirror = 2.0f * static_cast<float>(PI); // 2*PI
		SDL_Vertex vertices[tris];

		for (int i = 0; i < tris; i += 3) {
			// The upper bound of the triangle
			vertices[i].position = circle.center(); // 0, 3, 6, 9
			vertices[i].color = (FColour)colour;

			// Subtract 3 from tris so we don't operate on a triangle that is out of bounds

			// The lower right bound of the triangle
			vertices[i + 1].position.x = circle.x + (cos(i * mirror / (tris - 3)) * circle.r); // 1, 4, 7, 10
			vertices[i + 1].position.y = circle.y + (sin(i * mirror / (tris - 3)) * circle.r);
			vertices[i + 1].color = (FColour)colour;

			// The lower left bound of the triangle
			if (i > 0) {
				vertices[i - 1].position.x = circle.x + (cos(i * mirror / (tris - 3)) * circle.r); // 2, 5, 8, 11
				vertices[i - 1].position.y = circle.y + (sin(i * mirror / (tris - 3)) * circle.r);
				vertices[i - 1].color = (FColour)colour;
			}
		}

		SDL_RenderGeometry(renderer.get(), NULL, vertices, tris - 3, NULL, tris - 3);

	} else {
		int x = circle.r, y = 0;

		set_colour(colour);

		// Printing the initial point on the axes
		// after translation
		SDL_RenderPoint(renderer.get(), x + circle.x, circle.y);

		// When radius is zero only a single
		// point will be printed
		if (circle.r > 0) {
			SDL_RenderPoint(renderer.get(), -x + circle.x, circle.y);
			SDL_RenderPoint(renderer.get(), circle.x, -x + circle.y);
			SDL_RenderPoint(renderer.get(), circle.x, x + circle.y);
		}

		// Initialising the value of P
		int P = 1 - circle.r;
		while (x > y) {
			y++;

			// Mid-point is inside or on the perimeter
			if (P <= 0)
				P = P + 2*y + 1;
			// Mid-point is outside the perimeter
			else {
				x--;
				P = P + 2*y - 2*x + 1;
			}

			// All the perimeter points have already been printed
			if (x < y)
				break;

			// Printing the generated point and its reflection
			// in the other octants after translation
			SDL_RenderPoint(renderer.get(), x + circle.x, y + circle.y);
			SDL_RenderPoint(renderer.get(), -x + circle.x, y + circle.y);
			SDL_RenderPoint(renderer.get(), x + circle.x, -y + circle.y);
			SDL_RenderPoint(renderer.get(), -x + circle.x, -y + circle.y);

			// If the generated point is on the line x = y then
			// the perimeter points have already been printed
			if (x != y) {
				SDL_RenderPoint(renderer.get(), y + circle.x, x + circle.y);
				SDL_RenderPoint(renderer.get(), -y + circle.x, x + circle.y);
				SDL_RenderPoint(renderer.get(), y + circle.x, -x + circle.y);
				SDL_RenderPoint(renderer.get(), -y + circle.x, -x + circle.y);
			}
		}
	}
}

void Renderer::draw_polygon(const std::vector<Vector> &vertices, const Colour colour, const bool filled) {
	int n = vertices.size();

	if (filled) {
		std::vector<SDL_Vertex> verts(n);
		for (int i=0; i < n; i++) {
			verts[i].position = vertices[i];
			verts[i].color = (FColour)colour;
		}

		std::vector<int> indices((n-2)*3);
		for (int i=1; i < n - 1; i++) {
			indices[3*(i-1)] = 0;
			indices[3*i - 2] = i;
			indices[3*i - 1] = i+1;
		}

		SDL_RenderGeometry(renderer.get(), NULL, verts.data(), n, indices.data(), (n-2)*3);


	} else {
		set_colour(colour);

		int j = n - 1;
		for (int i=0; i < n; i++) {
			SDL_RenderLine(renderer.get(), vertices[j].x, vertices[j].y, vertices[i].x, vertices[i].y);
			j = i;
		}
	}
}

void Renderer::render_geometry_raw(const int num_vertices, const SDL_Vertex *vertices, const int num_indices, const int *indices) {
	SDL_RenderGeometry(renderer.get(), NULL, vertices, num_vertices, indices, num_indices);
}

void Renderer::render_geometry_raw(const int num_vertices, const SDL_Vertex *vertices, const int num_indices, const int *indices, Texture &texture) {
	SDL_RenderGeometry(renderer.get(), texture.texture.get(), vertices, num_vertices, indices, num_indices);
}

void Renderer::render_geometry(const std::vector<SDL_Vertex> &vertices) {
	render_geometry_raw(vertices.size(), &vertices[0], 0, NULL);
}

void Renderer::render_geometry(const std::vector<SDL_Vertex> &vertices, const std::vector<int> &indices) {
	render_geometry_raw(vertices.size(), &vertices[0], indices.size(), &indices[0]);
}

void Renderer::render_geometry(const std::vector<SDL_Vertex> &vertices, Texture &texture) {
	render_geometry_raw(vertices.size(), &vertices[0], 0, NULL, texture);
}

void Renderer::render_geometry(const std::vector<SDL_Vertex> &vertices, const std::vector<int> &indices, Texture &texture) {
	render_geometry_raw(vertices.size(), &vertices[0], indices.size(), &indices[0], texture);
}

void Renderer::render_geometry_sorted(const std::vector<SDL_Vertex> &vertices) {
	int n = vertices.size();
	std::vector<int> indices((n-2)*3);
	for (int i=1; i < n - 1; i++) {
		indices[3*(i-1)] = 0;
		indices[3*i - 2] = i;
		indices[3*i - 1] = i+1;
	}
	SDL_RenderGeometry(renderer.get(), NULL, vertices.data(), n, indices.data(), (n-2)*3);
}

void Renderer::render_geometry_sorted(const std::vector<SDL_Vertex> &vertices, Texture &texture) {
	int n = vertices.size();
	std::vector<int> indices((n-2)*3);
	for (int i=1; i < n - 1; i++) {
		indices[3*(i-1)] = 0;
		indices[3*i - 2] = i;
		indices[3*i - 1] = i+1;
	}
	SDL_RenderGeometry(renderer.get(), texture.texture.get(), vertices.data(), n, indices.data(), (n-2)*3);
}

void Renderer::destroy(SDL_Renderer *renderer) {
	SDL_DestroyRenderer(renderer);
	flog_info("Renderer destroyed successfully!");
}


Mouse::Mouse(const int needed_buttons) {
	int button;
	for (int i = 0; i < 5; i++) {
		if ((button = pow(2, i)) & needed_buttons) {
			buttons[button] = {static_cast<uint8_t>(i + 1)};
		}
	}
}

void Mouse::set_relative_mode(const bool val) {
	SDL_SetRelativeMouseMode(static_cast<SDL_bool>(val));
}

void Mouse::wrap_in_window(Window &window, const Vector &wrap_pos) {
	// The function calls window.wrap_mouse and updates the mouse position to wrap_pos
	window.wrap_mouse(wrap_pos);
	pos = wrap_pos;
}


bool Events::process_events(EventKeys *event_keys, Mouse *mouse, Fingers *fingers, std::function<bool(SDL_Event&)> event_handler) {
	// The function event handler should return true if the engine loop should not be run otherwise false
	if (event_keys) {
		for (auto &[key, value]: *event_keys) {
			value.pressed = false;
			value.released = false;
		}
	}

	if (mouse) {
		for (auto &[key, value]: mouse->buttons) {
			value.pressed = false;
			value.released = false;
		}
		mouse->vert_wheel = mouse->horz_wheel = 0;
	}

	if (fingers) {
		for (auto &[key, value]: *fingers) {
			value.pressed = false;
		}
	}

	while (SDL_PollEvent(&event)) {
		if (!(event_handler && event_handler(event))) {
			switch (event.type) {
				case SDL_EVENT_QUIT:
					running = false;
					break;
				case SDL_EVENT_KEY_DOWN:
					if (event_keys) {
						if (!event.key.repeat) {
							for (auto &[key, value]: *event_keys) {
								if ((event.key.key == value.primary) || (event.key.key == value.secondary)) {
									value.pressed = true;
									value.down = true;
								}
							}
						}
					}
					break;
				case SDL_EVENT_KEY_UP:
					if (event_keys) {
						if (!event.key.repeat) {
							for (auto &[key, value]: *event_keys) {
								if ((event.key.key == value.primary) || (event.key.key == value.secondary)) {
									value.released = true;
									value.down = false;
								}
							}
						}
					}
					break;
				case SDL_EVENT_MOUSE_MOTION:
					if (mouse) {
						mouse->pos.x = event.button.x;
						mouse->pos.y = event.button.y;
						break;
					}
					break;
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
					if (mouse) {
						for (auto &[key, value]: mouse->buttons) {
							if (event.button.button == value.id) {
								value.pressed = true;
								value.down = true;
							}
						}
					}
					break;
				case SDL_EVENT_MOUSE_BUTTON_UP:
					if (mouse) {
						for (auto &[key, value]: mouse->buttons) {
							if (event.button.button == value.id) {
								value.released = true;
								value.down = false;
							}
						}
					}
					break;
				case SDL_EVENT_MOUSE_WHEEL:
					if (mouse) {
						mouse->vert_wheel = event.wheel.y;
						mouse->horz_wheel = event.wheel.x;
					}
					break;
				case SDL_EVENT_FINGER_MOTION:
					if (fingers) {
						(*fingers)[event.tfinger.fingerID] = {
							event.tfinger.fingerID,
							{event.tfinger.x, event.tfinger.y},
							{event.tfinger.dx, event.tfinger.dy},
							false,
							event.tfinger.pressure
						};
					}
					break;
				case SDL_EVENT_FINGER_DOWN:
					if (fingers) {
						(*fingers)[event.tfinger.fingerID] = {
							event.tfinger.fingerID,
							{event.tfinger.x, event.tfinger.y},
							{event.tfinger.dx, event.tfinger.dy},
							true,
							event.tfinger.pressure
						};
					}
					break;
				case SDL_EVENT_FINGER_UP:
					if (fingers) {
						fingers->erase(event.tfinger.fingerID);
					}
					break;
			}
		}
	}

	return running;
}


bool Events::process_events(EventArgs event_args) {
	return process_events(event_args.event_keys, event_args.mouse, event_args.fingers, event_args.event_handler);
}


Surface::Surface(const IVector &size, const SDL_PixelFormat format):
	surface(managed_ptr<SDL_Surface>(SDL_CreateSurface(size.x, size.y, format), SDL_DestroySurface)) {
	if (surface.get() == nullptr)
		flog_error("Failed to create surface: {}", SDL_GetError());
	else {
		id = SURF_ID;
		flog_info("Surface created successfully![{}]", id);
		SURF_ID++;

		w = size.x;
		h = size.y;
	}
}

Surface::Surface(SDL_Surface *_surface): surface(managed_ptr<SDL_Surface>(_surface, SDL_DestroySurface)) {
	if (surface.get() == nullptr) {
		flog_error("Invalid surface");
	} else {
		w = surface.get()->w;
		h = surface.get()->h;	
	}
}

#ifdef IMAGE_ENABLED
Surface::Surface(const string &file):
	surface(managed_ptr<SDL_Surface>(IMG_Load(file.c_str()), SDL_DestroySurface)) {
	if (surface.get() == nullptr)
		flog_error("Failed to load surface: {}", SDL_GetError());
	else {
		id = SURF_ID;
		flog_info("Surface loaded successfully![{}]", id);
		SURF_ID++;

		w = surface.get()->w;
		h = surface.get()->h;
	}
}
#else
Surface::Surface([[maybe_unused]] const string &file):
	surface(managed_ptr<SDL_Surface>(nullptr, SDL_DestroySurface)) {
	image_function_not_implemented("Surface::Surface(const string &file)");
}
#endif /* IMAGE_ENABLED */

void Surface::set_blend_mode(const SDL_BlendMode blend_mode) {
	SDL_SetSurfaceBlendMode(surface.get(), blend_mode);
}

void Surface::set_colour_key(const uint32_t key, const bool enable) {
	SDL_SetSurfaceColorKey(surface.get(), enable, key);
}

void Surface::flip(const SDL_FlipMode flip_mode) {
	SDL_FlipSurface(surface.get(), flip_mode);
}

Surface Surface::convert_format(const SDL_PixelFormat format) {
	return SDL_ConvertSurface(surface.get(), format);
}

void Surface::convert_format_ip(const SDL_PixelFormat format) {
	surface = managed_ptr<SDL_Surface>(SDL_ConvertSurface(surface.get(), format), SDL_DestroySurface);
}

void Surface::blit(Surface &dst_surface, const IVector &ivec) {
	SDL_Rect src_rect = {0, 0, surface.get()->w, surface.get()->h};
	SDL_Rect dst_rect = {ivec.x, ivec.y, dst_surface.surface.get()->w, dst_surface.surface.get()->h};

	SDL_BlitSurface(surface.get(), &src_rect, dst_surface.surface.get(), &dst_rect);
}

void Surface::blit(Surface &dst_surface, const IRect &dst_rect) {
	SDL_Rect src_rect = {0, 0, surface.get()->w, surface.get()->h};
	SDL_Rect _dst_rect = dst_rect;

	SDL_BlitSurface(surface.get(), &src_rect, dst_surface.surface.get(), &_dst_rect);
}

void Surface::blit(Surface &dst_surface, const IRect &dst_rect, const IRect &src_rect) {
	SDL_Rect _src_rect = src_rect;
	SDL_Rect _dst_rect = dst_rect;

	SDL_BlitSurface(surface.get(), &_src_rect, dst_surface.surface.get(), &_dst_rect);
}

#ifdef IMAGE_ENABLED
void Surface::save(const string &file) {
	// This function saves the surface as png
	IMG_SavePNG(surface.get(), file.c_str());
}

void Surface::save(const string &file, const int quality) {
	// This function saves the surface as jpg
	// quality should be between 0 to 100
	IMG_SaveJPG(surface.get(), file.c_str(), quality);
}
#else
void Surface::save([[maybe_unused]] const string &file) {
	image_function_not_implemented("Surface::save(const string &file)");
}

void Surface::save(
	[[maybe_unused]] const string &file,
	[[maybe_unused]] const int quality
) {
	image_function_not_implemented(
		"Surface::save(const string &file, const int quality)"
	);
}
#endif /* IMAGE_ENABLED */


Texture::Texture(Renderer &renderer, SDL_Texture *_texture):
	texture(managed_ptr<SDL_Texture>(_texture, SDL_DestroyTexture)) {
	tex_renderer = &renderer;

	get_size();
}

Texture::Texture(Texture &&_texture): texture(std::move(_texture.texture)) {
	tex_renderer = _texture.tex_renderer;
	_texture.tex_renderer = nullptr;
	id = _texture.id;
	_texture.id = -1;
	w = _texture.w;
	h = _texture.h;
}

#ifdef IMAGE_ENABLED
Texture::Texture(Renderer &renderer, const string &file):
	texture(managed_ptr<SDL_Texture>(IMG_LoadTexture(renderer.renderer.get(), file.c_str()), SDL_DestroyTexture)) {
	tex_renderer = &renderer;
	if (texture.get() == nullptr)
		flog_error("Failed to load texture! ({}): {}", file, IMG_GetError());
	else {
		id = TEX_ID;
		flog_info("Texture loaded successfully![{}] ({})", id, file);
		TEX_ID++;
	}

	get_size();
}
#else
Texture::Texture(
	[[maybe_unused]] Renderer &renderer,
	[[maybe_unused]] const string &file
): texture(managed_ptr<SDL_Texture>(nullptr, SDL_DestroyTexture)) {
	image_function_not_implemented(
		"Texture::Texture(Renderer &renderer, const string &file)"
	);
}
#endif /* IMAGE_ENABLED */

Texture::Texture(Renderer &renderer, const Surface &surface):
	texture(managed_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer.renderer.get(), surface.surface.get()), SDL_DestroyTexture)) {
	tex_renderer = &renderer;
	if (texture.get() == nullptr)
		flog_error("Failed to create texture: {}", SDL_GetError());
	else {
		id = TEX_ID;
		flog_info("Texture created successfully![{}]", id);
		TEX_ID++;
	}

	get_size();
}

Texture::Texture(Renderer &renderer, const IVector &size, const SDL_PixelFormat format, const int access):
	texture(managed_ptr<SDL_Texture>(SDL_CreateTexture(renderer.renderer.get(), format, access, size.x, size.y), SDL_DestroyTexture)) {
	tex_renderer = &renderer;
	w = size.x;
	h = size.y;
	if (texture.get() == nullptr)
		flog_error("Failed to created texture: {}", SDL_GetError());
	else {
		id = TEX_ID;
		flog_info("Texture created successfully![{}]", id);
		TEX_ID++;
	}
}

IVector Texture::get_size() {
	// Also updates the w and h member variables
	w = SDL_GetNumberProperty(get_properties(), SDL_PROP_TEXTURE_WIDTH_NUMBER, 0);
	h = SDL_GetNumberProperty(get_properties(), SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0);

	return {w, h};
}

IRect Texture::get_rect() {
	return {0, 0, w, h};
}

SDL_PropertiesID Texture::get_properties() {
	return SDL_GetTextureProperties(texture.get());
}

int Texture::get_gl_texture_number() {
	return SDL_GetNumberProperty(
		get_properties(),
		SDL_PROP_TEXTURE_OPENGL_TEXTURE_NUMBER,
		0
	);
}

void Texture::set_colour_mod(const Colour &colour) {
	SDL_SetTextureColorMod(texture.get(), colour.r, colour.g, colour.b);
	if (colour.a != 255)
		SDL_SetTextureAlphaMod(texture.get(), colour.a);
}

void Texture::set_blend_mode(SDL_BlendMode blend_mode) {
	SDL_SetTextureBlendMode(texture.get(), blend_mode);
}

void Texture::update(const void *pixels, const int pitch) {
	SDL_UpdateTexture(texture.get(), NULL, pixels, pitch);
}

void Texture::update(const void *pixels, const int pitch, const IRect &rect) {
	const SDL_Rect r = rect;
	SDL_UpdateTexture(texture.get(), &r, pixels, pitch);
}

void Texture::update(const Surface &surface) {
	update(surface.surface->pixels, surface.surface->pitch);
}

void Texture::update(const Surface &surface, const IRect &rect) {
	update(surface.surface->pixels, surface.surface->pitch, rect);
}

void Texture::render(const Vector &vec) {
	render({vec, static_cast<Rect>(get_rect()).size()});
}

void Texture::render(const Rect &dst_rect) {
	render(dst_rect, get_rect());
}

void Texture::render(const Rect &dst_rect, const Rect &src_rect) {
	const SDL_FRect r1 = src_rect;
	const SDL_FRect r2 = dst_rect;
	SDL_RenderTexture(tex_renderer -> renderer.get(), texture.get(), &r1, &r2);
}

void Texture::render_rot(const Rect &dst_rect, const float angle, const Vector &center, const SDL_FlipMode flip) {
	render_rot(dst_rect, get_rect(), angle, center, flip);
}

void Texture::render_rot(const Rect &dst_rect, const Rect &src_rect, const float angle, const Vector &center, const SDL_FlipMode flip) {
	const SDL_FRect r1 = {src_rect.x, src_rect.y, src_rect.w, src_rect.h};
	const SDL_FRect r2 = {dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h};
	const SDL_FPoint p = {center.x, center.y};
	SDL_RenderTextureRotated(tex_renderer -> renderer.get(), texture.get(), &r1, &r2, angle, &p, flip);
}


std::vector<SDL_CameraID> Camera::get_available_devices() {
	int count;
	SDL_CameraID *dvcs = SDL_GetCameras(&count);
	if (!dvcs) {
		flog_error("Failed to get camera devices: {}", SDL_GetError());
	}
	std::vector<SDL_CameraID> devices(dvcs, dvcs + count);

	return devices;
}

SDL_CameraID Camera::select_device(const int id) {
	SDL_CameraID devid = get_available_devices().at(id);
	if (!devid)
		flog_error("No cameras available.");

	return devid;
}

Camera::Camera(const int id): camera(SDL_OpenCamera(select_device(id), NULL), SDL_CloseCamera) {
	if (camera == nullptr) {
		flog_error("Failed to open camera: {}", SDL_GetError());
	} else {
		SDL_CameraSpec spec;
		SDL_GetCameraFormat(camera.get(), &spec);
		size = {spec.width, spec.height};
		format = spec.format;
		permission_state = get_permission_state();
		flog_info("Camera opened successfully!");
	}
}

Camera::PERMISSION_STATE Camera::get_permission_state() {
	switch (SDL_GetCameraPermissionState(camera.get())) {
		case -1:
			permission_state = REJECTED;
			break;
		case 0:
			permission_state = PENDING;
			break;
		case 1:
			permission_state = APPROVED;
			break;
	}

	return permission_state;
}

bool Camera::is_new_frame_available() {
	// This function must be called before acquire_frame()
	// Otherwise acquire_frame() will keep returning the old surface
	// Also returns false if camera permission hasn't been granted or
	// if a new frame isn't available
	surface = SDL_AcquireCameraFrame(camera.get(), &time_stamp);
	return (surface != nullptr);
}

Surface& Camera::acquire_frame() {
	frame = std::make_unique<Surface>(surface);
	return *frame.get();
}

void Camera::release_frame() {
	frame.release();
	SDL_ReleaseCameraFrame(camera.get(), surface);
}
