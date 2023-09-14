#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <cstdlib>

#include "constants.h"
#include "core.h"
#ifdef IMAGE_ENABLED
#include <SDL2/SDL_image.h>
#endif /* IMAGE_ENABLED */
#ifdef MIXER_ENABLED
#include <SDL2/SDL_mixer.h>
#endif /* MIXER_ENABLED */
#ifdef TTF_ENABLED
#include <SDL2/SDL_ttf.h>
#endif /* TTF_ENABLED */
#ifdef NET_ENABLED
#include <SDL2/SDL_net.h>
#endif /* NET_ENABLED */


using namespace std;



// Globals
int SURF_ID = 0;
int TEX_ID = 0;

EventKeys EVENT_KEYS;
Fingers FINGERS;



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



// General functions
void start_text_input() {
	SDL_StartTextInput();
}


#ifdef __ANDROID__
void trigger_back_button() {
	SDL_AndroidBackButton();
}

void* get_activity() {
	void *activity;
	if ((activity = SDL_AndroidGetActivity()) == NULL) {
		SDL_LogError(0, "Failed to get the android activity: %s", SDL_GetError());
	}

	return activity;
}

string get_external_storage_path() {
	return SDL_AndroidGetExternalStoragePath();
}

int get_external_storage_state() {
	return SDL_AndroidGetExternalStorageState();
}

string get_internal_storage_path() {
	return SDL_AndroidGetInternalStoragePath();
}

void* get_jni_env() {
	void *jni;
	if ((jni = SDL_AndroidGetJNIEnv()) == 0) {
		SDL_LogError(0, "Failed to get JNIEnv: %s", SDL_GetError());
	}

	return jni;
}

bool get_permission(const string permission) {
	// Returns if the permission was granted or not
	return SDL_AndroidRequestPermission(permission.c_str());
}

void show_toast(const string message, const bool duration, const int gravity, const int offsetx, const int offsety) {
	// Set offset only when gravity is non-zero
	SDL_AndroidShowToast(message.c_str(), duration, gravity, offsetx, offsety);
}

#endif /* __ANDROID__ */



// Structs
Colour::operator SDL_Color() const {
	return {r, g, b, a};
}

Colour Colour::modulate(const double mod_r, const double mod_g, const double mod_b, const double mod_a) const {
	return {
		static_cast<Uint8>(r*mod_r),
		static_cast<Uint8>(g*mod_g),
		static_cast<Uint8>(b*mod_b),
		static_cast<Uint8>(a*mod_a)
	};
}


ostream& operator<<(ostream &os, IVector const &ivector) {
	cout << ivector.to_str();
		return os;
}

IVector operator+(const IVector &ivec1, const IVector &ivec2) {
	return {ivec1.x + ivec2.x, ivec1.y + ivec2.y};
}

IVector operator-(const IVector &ivec1, const IVector &ivec2) {
	return {ivec1.x - ivec2.x, ivec1.y - ivec2.y};
}

IVector operator*(const IVector &ivec, const double &val) {
	return {static_cast<int>(ivec.x*val), static_cast<int>(ivec.y*val)};
}

IVector operator/(const IVector &ivec, const double &val) {
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
	return {static_cast<double>(x), static_cast<double>(y)};
}

IVector::operator SDL_Point() const {
	return {x, y};
}

IVector::operator SDL_FPoint() const {
	return {static_cast<float>(x), static_cast<float>(y)};
}

const string IVector::to_str() const {
	return "IVector(" + to_string(x) + ", " + to_string(y) + ")";
}


ostream& operator<<(ostream &os, const Vector &vector) {
	cout << vector.to_str();
	return os;
}

Vector operator+(const Vector &vec1, const Vector &vec2) {
	return {vec1.x + vec2.x, vec1.y + vec2.y};
}

Vector operator-(const Vector &vec1, const Vector &vec2) {
	return {vec1.x - vec2.x, vec1.y - vec2.y};
}

Vector operator*(const Vector &vec, const double &val) {
	return {vec.x*val, vec.y*val};
}

Vector operator/(const Vector &vec, const double &val) {
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
	return {static_cast<float>(x), static_cast<float>(y)};
}

const string Vector::to_str() const {
	return "Vector(" + to_string(x) + ", " + to_string(y) + ")";
}

double Vector::magnitude_squared() const {
	return pow(x, 2) + pow(y, 2);
}

double Vector::magnitude() const {
	return pow(magnitude_squared(), 0.5);
}

Vector Vector::normalize() const {
	return {x / magnitude(), y / magnitude()};
}

void Vector::normalize_ip() {
	x = x / magnitude();
	y = y / magnitude();
}

Vector Vector::rotate_rad(const double &angle) const {
	return {x*cos(angle) - y*sin(angle), x*sin(angle) + y*cos(angle)};
}

void Vector::rotate_rad_ip(const double &angle) {
	double temp = x;
	x = x*cos(angle) - y*sin(angle);
	y = temp*sin(angle) + y*cos(angle);
}

Vector Vector::rotate(const double &angle) const {
	return rotate_rad(radians(angle));
}

void Vector::rotate_ip(const double &angle) {
	rotate_rad_ip(radians(angle));
}

double Vector::distance_to(const Vector &vec) const {
	return sqrt(pow(vec.x - x, 2) + pow(vec.y - y, 2));
}

double Vector::angle_rad() const {
	double temp = atan2(y, x);
	return (temp > 0) ? temp : 2*PI + temp;
}

double Vector::angle() const {
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
	double temp = this->distance_to(circle.center());

	if (temp > circle.radius()){
		x = circle.x + (x - circle.x)*circle.r/temp;
		y = circle.y + (y - circle.y)*circle.r/temp;
	}
}

Rect::Rect(int x, int y, int w, int h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

Rect::Rect(const Vector &pos, const Vector &size) {
	x = static_cast<int>(pos.x);
	y = static_cast<int>(pos.y);
	w = static_cast<int>(size.x);
	h = static_cast<int>(size.y);
}

ostream& operator<<(ostream &os, Rect const &rect) {
	cout << rect.to_str();
	return os;
}

Rect::operator SDL_Rect() const {
	return {x, y, w, h};
}

const string Rect::to_str() const {
	return "Rect(" + to_string(x) + ", " + to_string(y) + ", " + to_string(w) + ", " + to_string(h) + ")";
}

Vector Rect::size() const {
	return {static_cast<double>(w), static_cast<double>(h)};
}

void Rect::size(const Vector &vec) {
	w = vec.x;
	h = vec.y;
}

void Rect::scale(const Vector &vec) {
	w = w*vec.x;
	h = h*vec.y;
}

void Rect::scale(const double val) {
	w = w*val;
	h = h*val;
}

double Rect::half_width() const {
	return w / 2;
}

double Rect::half_height() const {
	return h / 2;
}

Vector Rect::half_size() const {
	return {half_width(), half_width()};
}

double Rect::top() const {
	return y;
}

void Rect::top(const double &val) {
	y = val;
}

double Rect::bottom() const {
	return y + h;
}

void Rect::bottom(const double &val) {
	y = val - h;
}

double Rect::left() const {
	return x;
}

void Rect::left(const double &val) {
	x = val;
}

double Rect::right() const {
	return x + w;
}

void Rect::right(const double &val) {
	x = val - w;
}

double Rect::centerx() const {
	return x + half_width();
}

void Rect::centerx(const double &val) {
	x = val - half_width();
}

double Rect::centery() const {
	return y + half_height();
}

void Rect::centery(const double &val) {
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
	if (((left() <= vec.x) && (vec.x <= right())) && ((top() <= vec.y) && (vec.y <= bottom()))) return true;
	return false;
}

bool Rect::collide_rect(const Rect &rect) const {
	if ((left() < rect.x + rect.w) && (right() > rect.x) && (top() < rect.y + rect.h) && (bottom() > rect.y)) return true;
	return false;
}

Rect Rect::clamp(const Rect &rect) const {
	// Clamps the rect within the rect passed
	Rect new_rect = {x, y, w, h};
	if (left() < rect.x) new_rect.left(rect.x);
	else if (right() > (rect.x + rect.w)) new_rect.right(rect.x + rect.w);
	if (top() < rect.y) new_rect.top(rect.y);
	else if (bottom() > (rect.y + rect.h)) new_rect.bottom(rect.y + rect.h);
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
	return {static_cast<int>(x + vec.x), static_cast<int>(y + vec.y), w, h};
}

void Rect::move_ip(const Vector &vec) {
	x += vec.x;
	y += vec.y;
}


Circle::Circle(int x, int y, int r) {
	this->x = x;
	this->y = y;
	this->r = r;
}

Circle::Circle(const Vector &vec, const int radius) {
	x = static_cast<int>(vec.x);
	y = static_cast<int>(vec.y);
	r = radius;
}

ostream& operator<<(ostream &os, const Circle &circle) {
	cout << circle.to_str();
	return os;
}

const string Circle::to_str() const {
	return "Circle(" + to_string(x) + ", " + to_string(y) + to_string(r) + ")";
}

double Circle::radius() const {
	return r;
}

void Circle::radius(const double radius) {
	r = radius;
}

Vector Circle::center() const {
	return {static_cast<double>(x), static_cast<double>(y)};
}

void Circle::center(const Vector &vec) {
	x = vec.x;
	y = vec.y;
}

bool Circle::collide_point(const Vector &vec) const {
	if (center().distance_to(vec) > r)
		return false;
	else
		return true;
}

bool Circle::collide_circle(const Circle &circle) const {
	if (circle.center().distance_to(center()) > circle.r + r)
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
	return {static_cast<int>(x + vec.x), static_cast<int>(y + vec.y), r};
}

void Circle::move_ip(const Vector &vec) {
	x += vec.x;
	y += vec.y;
}



// Classes
Engine::Engine() {
	SDL_Init(SDL_INIT_EVERYTHING);
#ifdef IMAGE_ENABLED
	IMG_Init(IMG_INIT_PNG);
#endif /* IMAGE_ENABLED */
#ifdef TTF_ENABLED
	TTF_Init();
#endif /* TTF_ENABLED */
#ifdef NET_ENABLED
	SDLNet_Init();
#endif /* TTF_ENABLED */
#ifdef NET_ENABLED
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
#endif /* NET_ENABLED */
	srand((unsigned) time(NULL)); // Create a seed for random number generation
	SDL_Log("Engine started!");
}

Engine::~Engine() {
#ifdef IMAGE_ENABLED
	IMG_Quit();
#endif /* IMAGE_ENABLED */
#ifdef TTF_ENABLED
	TTF_Quit();
#endif /* TTF_ENABLED */
#ifdef NET_ENABLED
	SDLNet_Quit();
#endif /* TTF_ENABLED */
#ifdef NET_ENABLED
	Mix_Quit();
#endif /* NET_ENABLED */
	SDL_Quit();
	SDL_Log("Engine stopped!");
}


double Clock::tick(double target_fps) {
	// The parameter target_fps should be 0 for unclamped fps
	if (target_fps) {
		target_ft = 1000/target_fps;
		raw_time = SDL_GetTicks64() - last_tick;
		delay = target_ft - raw_time;

		if (delay > 0)
			SDL_Delay((Uint32)delay);
	}

	current_time = SDL_GetTicks64();
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
	current_time = SDL_GetPerformanceCounter();
	double time_diff = (current_time - timeit_tick)/(double)SDL_GetPerformanceFrequency()*1000;
	timeit_tick = current_time;
	return time_diff;
}


Timer::Timer(double time) {
	// Time should be in seconds
	this->time = time;
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
	io = SDL_RWFromFile(file.c_str(), access_mode.c_str());

	if (io == NULL)
		SDL_LogError(0, "Failed to load file! (%s): %s", file.c_str(), SDL_GetError());
	else
		IS_LOADED = true;
}

int IO::read(void *ptr, const int max, const int size) {
	// The size parameter takes the size of the object to read in bytes
	// and the max parameter takes the maximum number of objects to read
	// Returns the number of objects read or -1 on error
	if (IS_LOADED)
		return SDL_RWread(io, ptr, size, max);
	SDL_LogWarn(1, "%s", "Failed to read: file not loaded successfully!");
	return -1;
}

string IO::read() {
	// Reads the whole file at once to a string
	int size;
	const int file_size = seek(0, RW_SEEK_END);
	SDL_Log("%i", file_size);
	char buffer[file_size + 1];
	seek(0, RW_SEEK_SET);

	if ((size = read(buffer, file_size)) == file_size) {
		buffer[size] = '\0';
		return string(buffer);
	}
	return NULL;
}

string IO::read(const int max) {
	// Reads the next max number of chars from the file to a string
	int size;
	char buffer[max + 1];

	if ((size = read(buffer, max)) >= 0) {
		buffer[size] = '\0';
		return string(buffer);
	}
	return NULL;
}

void IO::write(const void *ptr, const int num, const int size) {
	// The size parameter takes the size of the object to read in bytes
	// and the num parameter takes the number of objects to write
	// Returns the numer of objects written
	if (!IS_LOADED)
		SDL_LogWarn(1, "%s", "Failed to write: file not loaded successfully!");
	else if (SDL_RWwrite(io, ptr, size, num) < num)
		SDL_LogError(0, "Failed to write all the objects: %s", SDL_GetError());
}

void IO::write(const string &data) {
	write(data.c_str(), data.length());
}

Sint64 IO::tell() {
	if (IS_LOADED)
		return SDL_RWtell(io);
	SDL_LogWarn(1, "%s", "Failed to tell: file not loaded successfully!");
	return -1;
}

Sint64 IO::seek(Sint64 offset, int whence) {
	if (IS_LOADED)
		return SDL_RWseek(io, offset, whence);
	SDL_LogWarn(1, "%s", "Failed to seek: file not loaded successfully!");
	return -1;
}

void IO::close() {
	SDL_RWclose(io);
}


Window::Window(string title, int screen_w, int screen_h, Uint32 flags, int posx, int posy):
	window(managed_ptr<SDL_Window>(SDL_CreateWindow(title.c_str(), posx, posy, screen_w, screen_h, flags), destroy)) {
	if (window.get() == NULL)
		SDL_LogError(0, "Failed to create window: %s", SDL_GetError());
	else
		SDL_Log("Window created successfully!");
}

void Window::wrap_mouse(const Vector &wrap_pos) {
	SDL_WarpMouseInWindow(window.get(), static_cast<int>(wrap_pos.x), static_cast<int>(wrap_pos.y));
}

Surface Window::get_window_surface() {
	return Surface(SDL_GetWindowSurface(window.get()));
}

void Window::destroy(SDL_Window *window) {
	SDL_DestroyWindow(window);
	SDL_Log("Window closed successfully!");
}


Renderer::Renderer(Window &window, Uint32 flags, int index):
	renderer(managed_ptr<SDL_Renderer>(SDL_CreateRenderer(window.window.get(), index, flags),destroy)) {
	if (renderer.get() == NULL)
		SDL_LogError(0, "Failed to create renderer: %s", SDL_GetError());
	else
		SDL_Log("Renderer created successfully!");
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

void Renderer::set_blend_mode(SDL_BlendMode blendmode) {
	SDL_SetRenderDrawBlendMode(renderer.get(), blendmode);
}

void Renderer::set_target() {
	SDL_SetRenderTarget(renderer.get(), NULL);
}

void Renderer::set_target(Texture &tex) {
	SDL_SetRenderTarget(renderer.get(), tex.texture.get());
}

void Renderer::set_logical_size(const Vector &size) {
	SDL_RenderSetLogicalSize(renderer.get(), static_cast<int>(size.x), static_cast<int>(size.y));
}

Vector Renderer::get_output_size() {
	int w, h;
	SDL_GetRendererOutputSize(renderer.get(), &w, &h);
	return {static_cast<double>(w), static_cast<double>(h)};
}

void Renderer::draw_point_raw(const Vector &point_pos) {
	SDL_RenderDrawPoint(renderer.get(), point_pos.x, point_pos.y);
}

void Renderer::draw_point(const Vector &point_pos, const Colour &colour) {
	set_colour(colour);
	draw_point_raw(point_pos);
}

void Renderer::draw_line_raw(const Vector &v1, const Vector &v2) {
	SDL_RenderDrawLine(renderer.get(), v1.x, v1.y, v2.x, v2.y);
}

void Renderer::draw_line(const Vector &v1, const Vector &v2, const Colour &colour) {
	set_colour(colour);
	draw_line_raw(v1, v2);
}

void Renderer::draw_rect_raw(const Rect &rect, int width) {
	if (width == 0) {
		SDL_Rect r = rect;
		SDL_RenderFillRect(renderer.get(), &r);
	} else if (width == 1) {
		SDL_Rect r = rect;
		SDL_RenderDrawRect(renderer.get(), &r);
	} else {
		SDL_Rect r1 = {rect.x - width, rect.y - width, rect.w + 2*width, width};
		SDL_Rect r2 = {rect.x - width, rect.y + rect.h, rect.w + 2*width, width};
		SDL_Rect r3 = {rect.x - width, rect.y, width, rect.h};
		SDL_Rect r4 = {rect.x + rect.w, rect.y, width, rect.h};
		SDL_RenderFillRect(renderer.get(), &r1);
		SDL_RenderFillRect(renderer.get(), &r2);
		SDL_RenderFillRect(renderer.get(), &r3);
		SDL_RenderFillRect(renderer.get(), &r4);
	}
}

void Renderer::draw_rect(const Rect &rect, const Colour &colour, int width) {
	set_colour(colour);
	draw_rect_raw(rect, width);
}

void Renderer::draw_circle(const Circle &circle, const Colour &colour, const bool filled) {
	set_colour(colour);

	if (filled) {
		constexpr int tris = 225; // Amount of triangles
		float mirror = 2.0f * static_cast<float>(PI); // 2*PI
		SDL_Vertex vertices[tris] = {0};

		for (int i = 0; i < tris; i += 3) {
			// The upper bound of the triangle
			vertices[i].position = circle.center(); // 0, 3, 6, 9
			vertices[i].color = colour;

			// Subtract 3 from tris so we don't operate on a triangle that is out of bounds

			// The lower right bound of the triangle
			vertices[i + 1].position.x = circle.x + (cos(i * mirror / (tris - 3)) * circle.r); // 1, 4, 7, 10
			vertices[i + 1].position.y = circle.y + (sin(i * mirror / (tris - 3)) * circle.r);
			vertices[i + 1].color = colour;

			// The lower left bound of the triangle
			if (i > 0) {
				vertices[i - 1].position.x = circle.x + (cos(i * mirror / (tris - 3)) * circle.r); // 2, 5, 8, 11
				vertices[i - 1].position.y = circle.y + (sin(i * mirror / (tris - 3)) * circle.r);
				vertices[i - 1].color = colour;
			}
		}

		SDL_RenderGeometry(renderer.get(), NULL, vertices, tris - 3, NULL, tris - 3);

	} else {
		int x = circle.r, y = 0;

		// Printing the initial point on the axes
		// after translation
		SDL_RenderDrawPoint(renderer.get(), x + circle.x, circle.y);

		// When radius is zero only a single
		// point will be printed
		if (circle.r > 0) {
			SDL_RenderDrawPoint(renderer.get(), -x + circle.x, circle.y);
			SDL_RenderDrawPoint(renderer.get(), circle.x, -x + circle.y);
			SDL_RenderDrawPoint(renderer.get(), circle.x, x + circle.y);
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
			SDL_RenderDrawPoint(renderer.get(), x + circle.x, y + circle.y);
			SDL_RenderDrawPoint(renderer.get(), -x + circle.x, y + circle.y);
			SDL_RenderDrawPoint(renderer.get(), x + circle.x, -y + circle.y);
			SDL_RenderDrawPoint(renderer.get(), -x + circle.x, -y + circle.y);

			// If the generated point is on the line x = y then
			// the perimeter points have already been printed
			if (x != y) {
				SDL_RenderDrawPoint(renderer.get(), y + circle.x, x + circle.y);
				SDL_RenderDrawPoint(renderer.get(), -y + circle.x, x + circle.y);
				SDL_RenderDrawPoint(renderer.get(), y + circle.x, -x + circle.y);
				SDL_RenderDrawPoint(renderer.get(), -y + circle.x, -x + circle.y);
			}
		}
	}
}

void Renderer::draw_polygon(const vector<Vector> vertices, const Colour colour, const bool filled) {
	int n = vertices.size();

	if (filled) {
		SDL_Vertex verts[n];
		for (int i=0; i < n; i++) {
			verts[i].position = vertices[i];
			verts[i].color = colour;
		}

		int indices[(n-2)*3];
		for (int i=1; i < n - 1; i++) {
			indices[3*(i-1)] = 0;
			indices[3*i - 2] = i;
			indices[3*i - 1] = i+1;
		}

		SDL_RenderGeometry(renderer.get(), NULL, verts, n, indices, (n-2)*3);


	} else {
		set_colour(colour);

		int j = n - 1;
		for (int i=0; i < n; i++) {
			SDL_RenderDrawLine(renderer.get(), vertices[j].x, vertices[j].y, vertices[i].x, vertices[i].y);
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

void Renderer::render_geometry(const vector<SDL_Vertex> &vertices) {
	render_geometry_raw(vertices.size(), &vertices[0], 0, NULL);
}

void Renderer::render_geometry(const vector<SDL_Vertex> &vertices, const vector<int> &indices) {
	render_geometry_raw(vertices.size(), &vertices[0], indices.size(), &indices[0]);
}

void Renderer::render_geometry(const vector<SDL_Vertex> &vertices, Texture &texture) {
	render_geometry_raw(vertices.size(), &vertices[0], 0, NULL, texture);
}

void Renderer::render_geometry(const vector<SDL_Vertex> &vertices, const vector<int> &indices, Texture &texture) {
	render_geometry_raw(vertices.size(), &vertices[0], indices.size(), &indices[0], texture);
}

void Renderer::render_geometry_sorted(const vector<SDL_Vertex> &vertices) {
	int n = vertices.size();
	int indices[(n-2)*3];
	for (int i=1; i < n - 1; i++) {
		indices[3*(i-1)] = 0;
		indices[3*i - 2] = i;
		indices[3*i - 1] = i+1;
	}
	SDL_RenderGeometry(renderer.get(), NULL, &vertices[0], n, indices, (n-2)*3);
}

void Renderer::render_geometry_sorted(const vector<SDL_Vertex> &vertices, Texture &texture) {
	int n = vertices.size();
	int indices[(n-2)*3];
	for (int i=1; i < n - 1; i++) {
		indices[3*(i-1)] = 0;
		indices[3*i - 2] = i;
		indices[3*i - 1] = i+1;
	}
	SDL_RenderGeometry(renderer.get(), texture.texture.get(), &vertices[0], n, indices, (n-2)*3);
}

void Renderer::destroy(SDL_Renderer *renderer) {
	SDL_DestroyRenderer(renderer);
	SDL_Log("Renderer destroyed successfully!");
}


Mouse::Mouse(const int needed_buttons) {
	int button;
	for (int i = 0; i < 5; i++) {
		if ((button = pow(2, i)) & needed_buttons) {
			buttons[button] = {static_cast<Uint8>(i + 1)};
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


bool Events::process_events(EventKeys *event_keys, Mouse *mouse, Fingers *fingers, function<bool(SDL_Event&)> event_handler) {
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
		if (!(event_handler and event_handler(event))) {
			switch (event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					if (event_keys) {
						if (!event.key.repeat) {
							for (auto &[key, value]: *event_keys) {
								if ((event.key.keysym.sym == value.primary) | (event.key.keysym.sym == value.secondary)) {
									value.pressed = true;
									value.down = true;
								}
							}
						}
					}
					break;
				case SDL_KEYUP:
					if (event_keys) {
						if (!event.key.repeat) {
							for (auto &[key, value]: *event_keys) {
								if ((event.key.keysym.sym == value.primary) | (event.key.keysym.sym == value.secondary)) {
									value.released = true;
									value.down = false;
								}
							}
						}
					}
					break;
				case SDL_MOUSEMOTION:
					if (mouse) {
						mouse->pos.x = event.button.x;
						mouse->pos.y = event.button.y;
						break;
					}
				case SDL_MOUSEBUTTONDOWN:
					if (mouse) {
						for (auto &[key, value]: mouse->buttons) {
							if (event.button.button == value.id) {
								value.pressed = true;
								value.down = true;
							}
						}
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (mouse) {
						for (auto &[key, value]: mouse->buttons) {
							if (event.button.button == value.id) {
								value.released = true;
								value.down = false;
							}
						}
					}
					break;
				case SDL_MOUSEWHEEL:
					if (mouse) {
						mouse->vert_wheel = event.wheel.preciseY;
						mouse->horz_wheel = event.wheel.preciseX;
					}
					break;
				case SDL_FINGERMOTION:
					if (fingers) {
						(*fingers)[event.tfinger.fingerId] = {
							event.tfinger.fingerId,
							{event.tfinger.x, event.tfinger.y},
							{event.tfinger.dx, event.tfinger.dy},
							false,
							event.tfinger.pressure
						};
					}
					break;
				case SDL_FINGERDOWN:
					if (fingers) {
						(*fingers)[event.tfinger.fingerId] = {
							event.tfinger.fingerId,
							{event.tfinger.x, event.tfinger.y},
							{event.tfinger.dx, event.tfinger.dy},
							true,
							event.tfinger.pressure
						};
					}
					break;
				case SDL_FINGERUP:
					if (fingers) {
						fingers->erase(event.tfinger.fingerId);
					}
					break;
			}
		}
	}

	return running;
}


Surface::Surface(const int width, const int height, const Uint32 flag, const int depth, const Uint32 format):
	surface(managed_ptr<SDL_Surface>(SDL_CreateRGBSurfaceWithFormat(flag, width, height, depth, format), SDL_FreeSurface)) {}

Surface::Surface(SDL_Surface *_surface): surface(managed_ptr<SDL_Surface>(_surface, SDL_FreeSurface)) {
	if (surface.get() == NULL)
		SDL_LogError(0, "Failed to create surface: %s", SDL_GetError());
	else {
		id = SURF_ID;
		SDL_Log("Surface created successfully![%i]", id);
		SURF_ID++;
	}
}

void Surface::set_blend_mode(const SDL_BlendMode blend_mode) {
	SDL_SetSurfaceBlendMode(surface.get(), blend_mode);
}

void Surface::set_colour_key(const Uint32 key, const bool enable) {
	SDL_SetColorKey(surface.get(), enable, key);
}

void Surface::blit(Surface &dst_surface, const IVector &ivec) {
	SDL_Rect src_rect = {0, 0, surface.get()->w, surface.get()->h};
	SDL_Rect dst_rect = {ivec.x, ivec.y, dst_surface.surface.get()->w, dst_surface.surface.get()->h};

	SDL_BlitSurface(surface.get(), &src_rect, dst_surface.surface.get(), &dst_rect);
}

void Surface::blit(Surface &dst_surface, const Rect &dst_rect) {
	SDL_Rect src_rect = {0, 0, surface.get()->w, surface.get()->h};
	SDL_Rect _dst_rect = dst_rect;

	SDL_BlitSurface(surface.get(), &src_rect, dst_surface.surface.get(), &_dst_rect);
}

void Surface::blit(Surface &dst_surface, const Rect &dst_rect, const Rect &src_rect) {
	SDL_Rect _src_rect = src_rect;
	SDL_Rect _dst_rect = dst_rect;

	SDL_BlitSurface(surface.get(), &_src_rect, dst_surface.surface.get(), &_dst_rect);
}

void Surface::save(const string &file) {
	// This function saves the surface as png
	IMG_SavePNG(surface.get(), file.c_str());
}

void Surface::save(const string &file, const int quality) {
	// This function saves the surface as jpg
	// quality should be between 0 to 100
	IMG_SaveJPG(surface.get(), file.c_str(), quality);
}


Texture::Texture(Renderer &renderer, SDL_Texture *_texture):
	texture(managed_ptr<SDL_Texture>(_texture, SDL_DestroyTexture)) {
	tex_renderer = &renderer;
	if (texture.get() == NULL)
		SDL_LogError(0, "Failed to load texture: %s", SDL_GetError());
	else {
		id = TEX_ID;
		SDL_Log("Texture loaded successfully![%i]", id);
		TEX_ID++;
	}

	SDL_QueryTexture(texture.get(), NULL, NULL, &w, &h);
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
		SDL_LogError(0, "Failed to load texture! (%s): %s", file.c_str(), IMG_GetError());
	else {
		id = TEX_ID;
		SDL_Log("Texture loaded successfully![%i] (%s)", id, file.c_str());
		TEX_ID++;
	}

	SDL_QueryTexture(texture.get(), NULL, NULL, &w, &h);
}
#endif /* IMAGE_ENABLED */

Texture::Texture(Renderer &renderer, Surface surface):
	texture(managed_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer.renderer.get(), surface.surface.get()), SDL_DestroyTexture)) {
	tex_renderer = &renderer;
	if (texture.get() == nullptr)
		SDL_LogError(0, "Failed to load texture: %s", SDL_GetError());
	else {
		id = TEX_ID;
		SDL_Log("Texture created successfully![%i]", id);
		TEX_ID++;
	}

	SDL_QueryTexture(texture.get(), NULL, NULL, &w, &h);
}

Texture::Texture(Renderer &renderer, const Vector &size, const Uint32 format, const int access):
	texture(managed_ptr<SDL_Texture>(SDL_CreateTexture(renderer.renderer.get(), format, access, size.x, size.y), SDL_DestroyTexture)) {
	tex_renderer = &renderer;
	w = size.x;
	h = size.y;
	if (texture.get() == nullptr)
		SDL_LogError(0, "Failed to load texture: %s", SDL_GetError());
	else {
		id = TEX_ID;
		SDL_Log("Texture created successfully![%i]", id);
		TEX_ID++;
	}
}

Rect Texture::get_rect() {
	return {0, 0, w, h};
}

void Texture::set_colour_mod(const Colour &colour) {
	SDL_SetTextureColorMod(texture.get(), colour.r, colour.g, colour.b);
	if (colour.a != 255)
		SDL_SetTextureAlphaMod(texture.get(), colour.a);
}

void Texture::set_blend_mode(SDL_BlendMode blend_mode) {
	SDL_SetTextureBlendMode(texture.get(), blend_mode);
}

void Texture::render(const Vector &vec) {
	render({vec, get_rect().size()});
}

void Texture::render(const Rect &dst_rect) {
	render(dst_rect, get_rect());
}

void Texture::render(const Rect &dst_rect, const Rect &src_rect) {
	const SDL_Rect r1 = src_rect;
	const SDL_Rect r2 = dst_rect;
	SDL_RenderCopy(tex_renderer -> renderer.get(), texture.get(), &r1, &r2);
}

void Texture::render_ex(const Rect &dst_rect, const double &angle, const Vector &center, const SDL_RendererFlip &flip) {
	render_ex(dst_rect, get_rect(), angle, center, flip);
}

void Texture::render_ex(const Rect &dst_rect, const Rect &src_rect, const double &angle, const Vector &center, const SDL_RendererFlip &flip) {
	const SDL_Rect r1 = {src_rect.x, src_rect.y, src_rect.w, src_rect.h};
	const SDL_Rect r2 = {dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h};
	const SDL_Point p = {(int)center.x, (int)center.y};
	SDL_RenderCopyEx(tex_renderer -> renderer.get(), texture.get(), &r1, &r2, angle, &p, flip);
}

/*
States::States(vector<State> &states) {
	for (State &state: states) {
		this->states[state.name] = state;
	}
}

bool States::is_active(string state) {
	return states[state].active;
}

void States::activate(string state) {
	states[state].active = true;
}

void States::inactivate(string state) {
	states[state].active = false;
}

string States::get_top_state() {
	string state;
	int p = -1;
	for (auto &[key, value]: states) {
		if (value.priority > p) state = key; 
	}
	return state;
}
*/
