#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <cstdlib>

#include "engine.h"


using namespace std;



// Globals
double PI = 3.141592653589793238463;
int TEX_ID = 0;
int FONT_ID = 0;
int SOUND_ID = 0;



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



// Main functions
void Init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	SDLNet_Init();
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);

	srand((unsigned) time(NULL)); // Create a seed for random number generation
}


void Quit() {
	IMG_Quit();
	TTF_Quit();
	SDLNet_Quit();
	Mix_Quit();
	SDL_Quit();
}



// General functions
void start_text_input() {
	SDL_StartTextInput();
}



// Structs
Colour::operator SDL_Color() const {
	return {r, g, b, a};
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

Vector Vector::clamp(const Rect &rect) {
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

Vector Vector::clamp(const Circle &circle) {
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


ostream& operator<<(ostream &os, Rect const &rect) {
	cout << rect.to_str();
	return os;
}

Rect::operator SDL_Rect() const {
	return {x, y, w, h};
}

const string Rect::to_str() const {
	return "Rect(" + to_string(x) + ", " + to_string(y) + to_string(w) + ", " + to_string(h) + ", " + ")";
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

string IO::read(const int max) {
	char buffer[max+1];
	if (IS_LOADED and read(buffer, max))
		return string(buffer);
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


Window::Window(string title, int screen_w, int screen_h, Uint32 flags, int posx, int posy) {
	window = SDL_CreateWindow(
		title.c_str(),
		posx,
		posy,
		screen_w,
		screen_h,
		flags
	);

	if (window == NULL)
		SDL_LogError(0, "Failed to create window: %s", SDL_GetError());
	else
		SDL_Log("Window created successfully!");
}

Window::~Window() {
	if (window != nullptr)
		destroy();
}

Window::Window(Window &&win) noexcept {
	win.window = nullptr;
}

Window& Window::operator=(Window &&win) noexcept {
	if (&win == this)
		return *this;
	if (window != nullptr)
		destroy();
	window = win.window;
	win.window = nullptr;
	return *this;
}

void Window::destroy() {
	SDL_DestroyWindow(window);
	SDL_Log("Window closed successfully!");
}


Renderer::Renderer(Window &window, int index, Uint32 flags) {
	renderer = SDL_CreateRenderer(window.window, index, flags);
	if (renderer == NULL)
		SDL_LogError(0, "Failed to create renderer: %s", SDL_GetError());
	else
		SDL_Log("Renderer created successfully!");
}

Renderer::~Renderer() {
	if (renderer != nullptr)
		destroy();
}

Renderer::Renderer(Renderer &&ren) noexcept {
	ren.renderer = nullptr;
}

Renderer& Renderer::operator=(Renderer &&ren) noexcept {
	if (&ren == this)
		return *this;
	if (renderer != nullptr)
		destroy();
	renderer = ren.renderer;
	ren.renderer = nullptr;
	return *this;
}

void Renderer::destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_Log("Renderer destroyed successfully!");
}

void Renderer::set_colour(const Colour &colour) {
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
}

void Renderer::clear(const Colour &colour) {
	set_colour(colour);
	SDL_RenderClear(renderer);
}

void Renderer::present() {
	SDL_RenderPresent(renderer);
}

void Renderer::set_blend_mode(SDL_BlendMode blendmode) {
	SDL_SetRenderDrawBlendMode(renderer, blendmode);
}

void Renderer::set_target() {
	SDL_SetRenderTarget(renderer, NULL);
}

void Renderer::set_target(Texture &tex) {
	SDL_SetRenderTarget(renderer, tex.texture);
}

void Renderer::set_logical_size(const Vector &size) {
	SDL_RenderSetLogicalSize(renderer, static_cast<int>(size.x), static_cast<int>(size.y));
}

Vector Renderer::get_output_size() {
	int w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);
	return {static_cast<double>(w), static_cast<double>(h)};
}

void Renderer::draw_point(const Vector &point_pos, const Colour &colour) {
	set_colour(colour);
	SDL_RenderDrawPoint(renderer, point_pos.x, point_pos.y);
}

void Renderer::draw_line(const Vector &v1, const Vector &v2, const Colour &colour) {
	set_colour(colour);
	SDL_RenderDrawLine(renderer, v1.x, v1.y, v2.x, v2.y);
}

void Renderer::draw_rect(const Rect &rect, const Colour &colour, int width) {
	set_colour(colour);
	if (width == 0) {
		SDL_Rect r = rect;
		SDL_RenderFillRect(renderer, &r);
	} else if (width == 1) {
		SDL_Rect r = rect;
		SDL_RenderDrawRect(renderer, &r);
	} else {
		SDL_Rect r1 = {rect.x - width, rect.y - width, rect.w + 2*width, width};
		SDL_Rect r2 = {rect.x - width, rect.y + rect.h, rect.w + 2*width, width};
		SDL_Rect r3 = {rect.x - width, rect.y, width, rect.h};
		SDL_Rect r4 = {rect.x + rect.w, rect.y, width, rect.h};
		SDL_RenderFillRect(renderer, &r1);
		SDL_RenderFillRect(renderer, &r2);
		SDL_RenderFillRect(renderer, &r3);
		SDL_RenderFillRect(renderer, &r4);
	}
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

		SDL_RenderGeometry(renderer, NULL, vertices, tris - 3, NULL, tris - 3);

	} else {
		int x = circle.r, y = 0;

		// Printing the initial point on the axes
		// after translation
		SDL_RenderDrawPoint(renderer, x + circle.x, circle.y);

		// When radius is zero only a single
		// point will be printed
		if (circle.r > 0) {
			SDL_RenderDrawPoint(renderer, -x + circle.x, circle.y);
			SDL_RenderDrawPoint(renderer, circle.x, -x + circle.y);
			SDL_RenderDrawPoint(renderer, circle.x, x + circle.y);
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
			SDL_RenderDrawPoint(renderer, x + circle.x, y + circle.y);
			SDL_RenderDrawPoint(renderer, -x + circle.x, y + circle.y);
			SDL_RenderDrawPoint(renderer, x + circle.x, -y + circle.y);
			SDL_RenderDrawPoint(renderer, -x + circle.x, -y + circle.y);

			// If the generated point is on the line x = y then
			// the perimeter points have already been printed
			if (x != y) {
				SDL_RenderDrawPoint(renderer, y + circle.x, x + circle.y);
				SDL_RenderDrawPoint(renderer, -y + circle.x, x + circle.y);
				SDL_RenderDrawPoint(renderer, y + circle.x, -x + circle.y);
				SDL_RenderDrawPoint(renderer, -y + circle.x, -x + circle.y);
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

		SDL_RenderGeometry(renderer, NULL, verts, n, indices, (n-2)*3);


	} else {
		set_colour(colour);

		int j = n - 1;
		for (int i=0; i < n; i++) {
			SDL_RenderDrawLine(renderer, vertices[j].x, vertices[j].y, vertices[i].x, vertices[i].y);
			j = i;
		}
	}
}


Mouse::Mouse(const vector<string> &needed_buttons) {
	for (const string &value: needed_buttons) {
		buttons[value] = {BUTTON_MAP[value]};
	}
}


bool Events::process_events(unordered_map<string, EventKey> *event_keys, Mouse *mouse, unordered_map<SDL_FingerID, Finger> *fingers, bool (*event_handler)(SDL_Event &)) {
	// The function event handler should return true if the engine loop should not be run otherwise true
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


Texture::Texture(Renderer &renderer, const string &file) {
	tex_renderer = &renderer;
	texture = IMG_LoadTexture(tex_renderer -> renderer, file.c_str());
	if (texture == NULL)
		SDL_LogError(0, "Failed to load texture! (%s): %s", file.c_str(), SDL_GetError());
	else {
		id = TEX_ID;
		SDL_Log("Texture loaded successfully![%i] (%s)", id, file.c_str());
		TEX_ID++;
	}

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

Texture::Texture(Renderer &renderer, SDL_Surface *surface) {
	tex_renderer = &renderer;
	texture = SDL_CreateTextureFromSurface(tex_renderer -> renderer, surface);
	SDL_FreeSurface(surface);
	if (texture == NULL)
		SDL_LogError(0, "Failed to load texture: %s", SDL_GetError());
	else {
		id = TEX_ID;
		SDL_Log("Texture created successfully![%i]", id);
		TEX_ID++;
	}

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

Texture::Texture(Renderer &renderer, const Vector &size, const Uint32 format, const int access) {
	tex_renderer = &renderer;
	w = size.x;
	h = size.y;
	texture = SDL_CreateTexture(tex_renderer -> renderer, format, access, size.x, size.y);
	if (texture == NULL)
		SDL_LogError(0, "Failed to load texture: %s", SDL_GetError());
	else {
		id = TEX_ID;
		SDL_Log("Texture created successfully![%i]", id);
		TEX_ID++;
	}
}

Texture::~Texture() {
	if (texture != nullptr) {
		destroy();
	}
}

Texture::Texture(Texture &&tex) noexcept {
	tex.texture = nullptr;
}

Texture& Texture::operator=(Texture &&tex) noexcept {
	if (&tex == this) return *this;
	if (texture != nullptr) destroy();
	texture = tex.texture;
	tex.texture = nullptr;
	return *this;
}

Rect Texture::get_rect() {
	return {0, 0, w, h};
}

void Texture::set_colour_mod(const Colour &colour) {
	SDL_SetTextureColorMod(texture, colour.r, colour.g, colour.b);
	if (colour.a != 255) SDL_SetTextureAlphaMod(texture, colour.a);
}

void Texture::set_blend_mode(SDL_BlendMode blend_mode) {
	SDL_SetTextureBlendMode(texture, blend_mode);
}

void Texture::render(const Rect &dst_rect) {
	const SDL_Rect src_rect = get_rect();
	const SDL_Rect r2 = dst_rect;
	SDL_RenderCopy(tex_renderer -> renderer, texture, &src_rect, &r2);
}

void Texture::render(const Rect &dst_rect, const Rect &src_rect) {
	const SDL_Rect r1 = {src_rect.x, src_rect.y, src_rect.w, src_rect.h};
	const SDL_Rect r2 = {dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h};
	SDL_RenderCopy(tex_renderer -> renderer, texture, &r1, &r2);
}

void Texture::render_ex(const Rect &dst_rect, const double &angle, const Vector &center, const SDL_RendererFlip &flip) {
	const SDL_Rect r1 = {0, 0, w, h};
	const SDL_Rect r2 = {dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h};
	const SDL_Point p = {(int)center.x, (int)center.y};
	SDL_RenderCopyEx(tex_renderer -> renderer, texture, &r1, &r2, angle, &p, flip);
}

void Texture::render_ex(const Rect &dst_rect, const Rect &src_rect, const double &angle, const Vector &center, const SDL_RendererFlip &flip) {
	const SDL_Rect r1 = {src_rect.x, src_rect.y, src_rect.w, src_rect.h};
	const SDL_Rect r2 = {dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h};
	const SDL_Point p = {(int)center.x, (int)center.y};
	SDL_RenderCopyEx(tex_renderer -> renderer, texture, &r1, &r2, angle, &p, flip);
}

void Texture::destroy() {
	SDL_DestroyTexture(texture);
	SDL_Log("Texture destroyed successfully![%i]", id);
}


Font::Font(const string &file, const int size) {
	font = TTF_OpenFont(file.c_str(), size);

	if (font == NULL)
		SDL_LogError(0, "Failed to load font! (%s): %s", file.c_str(), SDL_GetError());
	else {
		id = FONT_ID;
		SDL_Log("Font loaded successfully![%i] (%s)", id, file.c_str());
		FONT_ID++;
	}
}

Font::~Font() {
	if (font != nullptr) destroy(); 
}

Font::Font(Font &&fnt) noexcept {
	fnt.font = nullptr;
}

Font& Font::operator=(Font &&fnt) noexcept {
	if (&fnt == this) return *this;
	if (font != nullptr) destroy();
	font = fnt.font;
	fnt.font = nullptr;
	return *this;
}

Texture Font::create_text(Renderer &renderer, const string &text, const Colour &colour, const int &quality, const bool &wrap_text, const Uint32 &wrap_length, const Colour &background_colour) {
	SDL_Surface *text_surf;
	switch (quality) {
		case 0: 
			if (wrap_text) 
				text_surf = TTF_RenderUTF8_Solid_Wrapped(font, text.c_str(), {colour.r, colour.g, colour.b, colour.a}, wrap_length);
			else text_surf = TTF_RenderUTF8_Solid(font, text.c_str(), {colour.r, colour.g, colour.b, colour.a});
		case 1:
			if (wrap_text)
				text_surf = TTF_RenderUTF8_Shaded_Wrapped(
					font,
					text.c_str(),
					{colour.r, colour.g, colour.b, colour.a},
					{background_colour.r, background_colour.g, background_colour.b, background_colour.a},
					wrap_length
				);
			else
				text_surf = TTF_RenderUTF8_Shaded(
					font,
					text.c_str(),
					{colour.r, colour.g, colour.b, colour.a},
					{background_colour.r, background_colour.g, background_colour.b, background_colour.a}
				);
		case 2:
			if (wrap_text)
				text_surf = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), {colour.r, colour.g, colour.b, colour.a}, wrap_length);
			else
				text_surf = TTF_RenderUTF8_Blended(font, text.c_str(), {colour.r, colour.g, colour.b, colour.a});
	}

	Texture texture = Texture(renderer, text_surf);
	return texture;
}

Font::FontAtlas Font::create_atlas(Renderer &renderer, const string chars, const int quality, const Uint8 &alpha) {
	TTF_SetFontKerning(font, 0);
	
	FontAtlas font_atlas;
	font_atlas.atlas = this->create_text(renderer, chars, {255, 255, 255, alpha}, quality);
	
	int x = 0;
	int w;
	int h;
	for (int i = 0; i < chars.length(); i++) {
		char ch[] = { chars[i], '\0' };
		TTF_SizeUTF8(font, ch, &w, &h);
		font_atlas.data[chars[i]] = {x, 0, w, font_atlas.atlas.h};
		x += w;
	}
	TTF_SetFontKerning(font, 1);

	return font_atlas;
}

void Font::draw_text(FontAtlas &font_atlas, const string &text, const Colour &colour, const int &x, const int &y, const double size) {
	// Alpha value is ignored in the colour argument. It should be set during creation of the atlas.
	font_atlas.atlas.set_colour_mod(colour);
	int cx = x;
	for (auto &ch: text) {
		Rect *r = &font_atlas.data[ch];
		font_atlas.atlas.render({cx, y, static_cast<int>(r->w*(size/r->h)), static_cast<int>(size)}, font_atlas.data[ch]);
		cx += r->w*(size/r->h);
	}
}

void Font::destroy() {
	TTF_CloseFont(font);
	SDL_Log("Font destroyed successfully![%i]", id);
}


SpriteSheet::SpriteSheet(Renderer &renderer, const string &file, const int &column, const int &row): texture(Texture(renderer, file)) {
	tile_x = column; tile_y = row;
	total_tiles = tile_x*tile_y;
	src_rect = {0, 0, texture.w, texture.h};
	tile_w = src_rect.w/tile_x; tile_h = src_rect.h/tile_y;
}

SpriteSheet::~SpriteSheet() {
	if (texture.texture != nullptr) destroy(); 
}

SpriteSheet::SpriteSheet(SpriteSheet &&ss) noexcept {
	ss.texture.texture = nullptr;
}

SpriteSheet& SpriteSheet::operator=(SpriteSheet &&ss) noexcept {
	if (&ss == this) return *this;
	if (texture.texture != nullptr) destroy();
	texture.texture = ss.texture.texture;
	ss.texture.texture = nullptr;
	return *this;
}

void SpriteSheet::set_src_rect(const Rect &src_rect) {
	this->src_rect = src_rect;
	tile_w = src_rect.w/tile_x; tile_h = src_rect.h/tile_y;
}

void SpriteSheet::draw_sprite(const Rect &dst_rect, const int &column, const int &row) {
	texture.render(dst_rect, {src_rect.x + tile_w*column, src_rect.y + tile_h*row, tile_w, tile_h});
}

void SpriteSheet::destroy() {
	texture.destroy();
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


Packet::Packet(const int maxlen) {
	this->maxlen = maxlen;
	packet = SDLNet_AllocPacket(maxlen);

	for_sending = false;
}

Packet::Packet(const int maxlen, IPaddress &destination) {
	this->maxlen = maxlen;
	packet = SDLNet_AllocPacket(maxlen);

	this->destination = destination;

	for_sending = true;
}

Packet::~Packet() {
	if (packet != nullptr) destroy(); 
}

Packet::Packet(Packet &&pk) noexcept {
	pk.packet = nullptr;
}

Packet& Packet::operator=(Packet &&pk) noexcept {
	if (&pk == this) return *this;
	if (packet != nullptr) destroy();
	packet = pk.packet;
	pk.packet = nullptr;
	return *this;
}

void Packet::clear_data() {
	data.clear();
}

void Packet::set_data(char *val) {
	clear_data();
	_serialized_data = string(val);
	int end = 0;
	for (int i = 0; i < _serialized_data.size(); i++) {
		if (_serialized_data[i] == DELIMITER) {
			data.push_back(_serialized_data.substr(end, i - end));
			end = i + 1;
		}
	}
}

Uint8* Packet::get_data() {
	// Every time this function is called the data becomes empty

	_serialized_data = "";
	for (string val: data) {
		_serialized_data += val + DELIMITER;
	}
	clear_data();
	if (_serialized_data.size() > maxlen)
		SDL_LogError(0, "Packet overflow! Data won't be send.");

	return (Uint8 *)_serialized_data.c_str();
}

void Packet::ready() {
	// This function shouldn't be called explicitly by the user.

	if (for_sending) {
		packet->address.host = destination.host;
		packet->address.port = destination.port;
		packet->data = get_data();
		packet->len = strlen((char *)packet->data) + 1;
	} else
		SDL_LogError(0, "This packet is not for sending.");
}

void Packet::set() {
	// This function shouldn't be called explicitly by the user.

	if (for_sending)
		SDL_LogError(0, "This packet is not for receiving.");
	else
		set_data((char *)packet->data);
}

string Packet::get_next_val() {
	_val = data.front();
	data.erase(data.begin());
	return _val;
}

void Packet::destroy() {
	SDLNet_FreePacket(packet);
}

Packet& operator<<(Packet &packet, const string val) {
	packet.data.push_back(val);
	return packet;
}

Packet& operator<<(Packet &packet, const char *val) {
	packet << string(val);
	return packet;
}

Packet& operator<<(Packet &packet, const int val) {
	packet << to_string(val);
	return packet;
}

Packet& operator<<(Packet &packet, const double val) {
	packet << to_string(val);
	return packet;
}

Packet& operator<<(Packet &packet, const Uint8 val) {
	packet << to_string(val);
	return packet;
}

Packet& operator<<(Packet &packet, const Colour &colour) {
	packet << colour.r << colour.g << colour.b << colour.a;
	return packet;
}

Packet& operator<<(Packet &packet, const Vector &vec) {
	packet << vec.x << vec.y;
	return packet;
}

Packet& operator<<(Packet &packet, const Rect &rect) {
	packet << rect.x << rect.y << rect.w << rect.h;
	return packet;
}

Packet& operator>>(Packet &packet, string &val) {
	val = packet.get_next_val();
	return packet;
}

Packet& operator>>(Packet &packet, char *val) {
	val = const_cast<char *>(packet.get_next_val().c_str());
	return packet;
}

Packet& operator>>(Packet &packet, int &val) {
	val = stoi(packet.get_next_val());
	return packet;
}

Packet& operator>>(Packet &packet, double &val) {
	val = stod(packet.get_next_val());
	return packet;
}

Packet& operator>>(Packet &packet, Uint8 &val) {
	val = stoi(packet.get_next_val());
	return packet;
}

Packet& operator>>(Packet &packet, Colour &colour) {
	packet >> colour.r >> colour.g >> colour.b >> colour.a;
	return packet;
}

Packet& operator>>(Packet &packet, Vector &vec) {
	packet >> vec.x >> vec.y;
	return packet;
}

Packet& operator>>(Packet &packet, Rect &rect) {
	packet >> rect.x >> rect.y >> rect.w >> rect.h;
	return packet;
}


UDPSocket::UDPSocket(const int port) {
	socket = SDLNet_UDP_Open(port);
}

UDPSocket::~UDPSocket() {
	if (socket != nullptr) destroy(); 
}

UDPSocket::UDPSocket(UDPSocket &&sock) noexcept {
	sock.socket = nullptr;
}

UDPSocket& UDPSocket::operator=(UDPSocket &&sock) noexcept {
	if (&sock == this) return *this;
	if (socket != nullptr) destroy();
	socket = sock.socket;
	sock.socket = nullptr;
	return *this;
}

bool UDPSocket::send(Packet &packet, const int channel) {
	packet.ready();
	if (SDLNet_UDP_Send(socket, channel, packet.packet)) {
		return true;
	} else {
		SDL_LogError(0, "Failed to send packet: %s", SDLNet_GetError());
		return false;
	}
}

bool UDPSocket::recv(Packet &packet) {
	int val = SDLNet_UDP_Recv(socket, packet.packet);
	if (val == 1) {
		packet.set();
		return true;
	} else if (val == 0)
		return false;
	else
		SDL_LogError(0, "Packet not received: %s", SDLNet_GetError());
	return false;
}

void UDPSocket::destroy() {
	SDLNet_UDP_Close(socket);
}


TCPSocket::TCPSocket(IPaddress &ip) {
	if ((socket = SDLNet_TCP_Open(&ip)) == NULL)
		SDL_LogError(0, "Failed to create socket: %s", SDLNet_GetError());
	else
		SDL_Log("Socket created successfully!");
}

TCPSocket::~TCPSocket() {
	if (socket != nullptr)
		destroy();
}

TCPSocket::TCPSocket(TCPSocket &&sock) noexcept {
	sock.socket = nullptr;
}

TCPSocket& TCPSocket::operator=(TCPSocket &&sock) noexcept {
	if (&sock == this)
		return *this;
	if (socket != nullptr)
		destroy();
	socket = sock.socket;
	sock.socket = nullptr;
	return *this;
}

bool TCPSocket::accept(TCPSocket &sock) {
	if ((sock.socket = SDLNet_TCP_Accept(socket)))
		return 1;
	return 0;
}

IPaddress* TCPSocket::get_peer_address() {
	if (!(ip = SDLNet_TCP_GetPeerAddress(socket)))
		SDL_LogError(0, "Failed to get peer address: %s", SDLNet_GetError());
	return ip;
}

void TCPSocket::send(const char buffer[], int size) {
	_val = SDLNet_TCP_Send(socket, buffer, size);
	if (_val < 0)
		SDL_LogError(0, "Invalid socket: %s", SDLNet_GetError());
	else if (_val < size)
		SDL_LogWarn(1, "Full data not sent: %s", SDLNet_GetError());
}

void TCPSocket::send(string &data) {
	send(data.data(), data.length() + 1);
}

int TCPSocket::recv(char buffer[], const int size) {
	return SDLNet_TCP_Recv(socket, buffer, size);
}

void TCPSocket::destroy() {
	SDLNet_TCP_Close(socket);
	SDL_Log("Socket closed successfully!");
}


void NetUtils::resolve_host(IPaddress &IP, const int port, const string host) {
	if (SDLNet_ResolveHost(&IP, host.c_str(), port) < 0)
		SDL_LogError(0, "Failed to resolve host: %s", SDL_GetError());
}


void Mixer::open_audio_device(int frequency, Uint16 format, int channels, int chunksize) {
	if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0) {
		SDL_LogError(0, "Failed to open audio device: %s", Mix_GetError());
	}
}


Sound::Sound(const string &file) {
	music = Mix_LoadMUS(file.c_str());

	id = SOUND_ID;
	SDL_Log("Sound loaded successfully![%i] (%s)", id, file.c_str());
	SOUND_ID++;
}

Sound::~Sound() {
	if (music != nullptr) destroy(); 
}

Sound::Sound(Sound &&sound) noexcept {
	sound.music = nullptr;
}

Sound& Sound::operator=(Sound &&sound) noexcept {
	if (&sound == this) return *this;
	if (music != nullptr) destroy();
	music = sound.music;
	sound.music = nullptr;
	return *this;
}

void Sound::play(int loop) {
	Mix_PlayMusic(music, loop);
}

float Sound::volume() {
	// Returns the current volume
	return Mix_VolumeMusic(-1)/MIX_MAX_VOLUME;
}

void Sound::volume(float volume) {
	// The value of the parameter volume should be between 0 to 1
	Mix_VolumeMusic((int)(volume*MIX_MAX_VOLUME));
}

void Sound::pause() {
	if (is_paused) {
		SDL_LogWarn(1, "Sound is already paused!");
	} else {
		Mix_PauseMusic();
		is_paused = true;
	}
}

void Sound::resume() {
	if (is_paused) {
		Mix_ResumeMusic();
		is_paused = false;
	} else {
		SDL_LogWarn(1, "Sound is not paused!");
	}
}

void Sound::toggle() {
	if (is_paused)
		Mix_ResumeMusic();
	else
		Mix_PauseMusic();
	is_paused = not is_paused;
}

void Sound::destroy() {
	Mix_FreeMusic(music);
	SDL_Log("Sound destroyed successfully![%i]", id);
}
