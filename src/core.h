#ifndef SUPERNOVA_CORE_H
#define SUPERNOVA_CORE_H


#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

#include <SDL3/SDL.h>


using string = std::string;



// Forward Declarations
struct Colour;
struct FColour;
struct IVector;
struct Vector;
struct IRect;
struct Rect;
struct Circle;
struct EventKey;
struct MouseButton;
struct Finger;
class Clock;
class Window;
class Renderer;
class Surface;
class Texture;
class Mouse;
class Events;
class Camera;



// Helper functions

// Degree to radians
double radians(const double angle);
// Radians to degrees
double degrees(const double angle);

// Generates a random integer b/w 0 to end (0 included and end excluded).
int randint(const int end);
// Generates a random integer b/w start to end
// (start included and end excluded).
int randint(const int start, const int end);



// Templates
template<typename T> using managed_ptr = std::unique_ptr<T, void(*)(T*)>;



// General functions
void start_text_input();

#ifdef __ANDROID__
void trigger_back_button();

void* get_activity();

string get_external_storage_path();
int get_external_storage_state();

string get_internal_storage_path();

void* get_jni_env();

// Returns if the permission was granted or not
bool get_permission(const string permission);

// Set offsetx and offsety only when gravity is non-zero
void show_toast(
	const string message,
	const bool duration=0,
	const int gravity=0,
	const int offsetx=0,
	const int offsety=0
);
#endif /* __ANDROID__ */



// Structs
struct Colour {
	uint8_t r, g, b;
	uint8_t a = 255;

	friend std::ostream& operator<<(std::ostream &os, const Colour &colour);
	friend Colour operator/(const Colour &colour, const float val);
	friend Colour operator*(const Colour &colour, const float &val);
	friend void operator*=(Colour &colour1, const Colour &colour2);
	friend void operator/=(Colour &colour, const float val);

	explicit operator FColour() const;
	operator SDL_Color() const;

	const string to_str() const;

	// The values of mod_r, mod_g, mod_b and mod_a should lie b/w 0 to 1
	Colour modulate(
		const float mod_r=1,
		const float mod_g=1,
		const float mod_b=1,
		const float mod_a=1
	) const;
};


struct FColour {
	float r, g, b;
	float a = 1.0f;

	friend std::ostream& operator<<(std::ostream &os, const Colour &fcolour);
	friend FColour operator*(const FColour &fcolour, const float val);
	friend FColour operator/(const FColour &fcolour, const float val);
	friend void operator*=(FColour &fcolour, const float val);
	friend void operator/=(FColour &fcolour, const float val);

	explicit operator Colour() const;
	operator SDL_FColor() const;

	const string to_str() const;

	// The values of mod_r, mod_g, mod_b and mod_a should lie b/w 0 to 1
	FColour modulate(
		const float mod_r=1,
		const float mod_g=1,
		const float mod_b=1,
		const float mod_a=1
	) const;
};


struct IVector {
	int x, y;

	friend std::ostream& operator<<(std::ostream &os, const IVector &ivector);
	friend IVector operator+(const IVector &ivec1, const IVector &ivec2);
	friend IVector operator-(const IVector &ivec1, const IVector &ivec2);
	friend IVector operator*(const IVector &ivec, const float &val);
	friend IVector operator/(const IVector &ivec, const float &val);
	friend void operator+=(IVector &ivec1, const IVector &ivec2);
	friend void operator-=(IVector &ivec1, const IVector &ivec2);
	friend void operator*=(IVector &ivec1, const IVector &ivec2);
	friend void operator/=(IVector &ivec1, const IVector &ivec2);

	operator Vector() const;
	operator SDL_Point() const;
	operator SDL_FPoint() const;

	const string to_str() const;
};


struct Vector {
	float x, y;

	friend std::ostream& operator<<(std::ostream &os, const Vector &vector);
	friend Vector operator+(const Vector &vec1, const Vector &vec2);
	friend Vector operator-(const Vector &vec1, const Vector &vec2);
	friend Vector operator*(const Vector &vec, const float &val);
	friend Vector operator/(const Vector &vec, const float &val);
	friend void operator+=(Vector &vec1, const Vector &vec2);
	friend void operator-=(Vector &vec1, const Vector &vec2);
	friend void operator*=(Vector &vec1, const Vector &vec2);
	friend void operator/=(Vector &vec1, const Vector &vec2);

	operator IVector() const;
	operator SDL_Point() const;
	operator SDL_FPoint() const;
	
	const string to_str() const;

	float magnitude_squared() const;
	float magnitude() const;

	Vector normalize() const;
	void normalize_ip();
	Vector rotate_rad(const float &angle) const;
	void rotate_rad_ip(const float &angle);
	Vector rotate(const float &angle) const; // In degrees
	void rotate_ip(const float &angle); // In degrees
	float distance_to_squared(const Vector &vec) const;
	float distance_to(const Vector &vec) const;
	float angle_rad() const;
	float angle() const; // In degrees
	Vector clamp(const Rect &rect) const;
	Vector clamp(const Circle &circle) const;
	void clamp_ip(const Rect &rect);
	void clamp_ip(const Circle &circle);
};


struct IRect {
	int x, y, w, h;

	IRect() {};
	IRect(int x, int y, int w, int h);
	IRect(const IVector &pos, const IVector &size);

	friend std::ostream& operator<<(std::ostream &os, const Rect &rect);

	operator Rect() const;
	operator SDL_Rect() const;

	const string to_str() const;
};


struct Rect {
	float x, y, w, h;

	Rect() {};
	Rect(float x, float y, float w, float h);
	Rect(const Vector &pos, const Vector &size);

	friend std::ostream& operator<<(std::ostream &os, const Rect &rect);

	operator SDL_FRect() const;

	const string to_str() const;

	Vector size() const;
	void size(const Vector &vec);
	void scale(const Vector &vec);
	// Scales the height and width by the given factor
	void scale(const float val);
	float half_width() const;
	float half_height() const;
	Vector half_size() const;

	float top() const;
	void top(const float &val);
	float bottom() const;
	void bottom(const float &val);
	float left() const;
	void left(const float &val);
	float right() const;
	void right(const float &val);
	float centerx() const;
	void centerx(const float &val);
	float centery() const;
	void centery(const float &val);
	Vector topleft() const;
	void topleft(const Vector &vec);
	Vector topright() const;
	void topright(const Vector &vec);
	Vector bottomleft() const;
	void bottomleft(const Vector &vec);
	Vector bottomright() const;
	void bottomright(const Vector &vec);
	Vector center() const;
	void center(const Vector &vec);
	Vector midtop() const;
	void midtop(const Vector &vec);
	Vector midbottom() const;
	void midbottom(const Vector &vec);
	Vector midleft() const;
	void midleft(const Vector &vec);
	Vector midright() const;
	void midright(const Vector &vec);

	bool collide_point(const Vector &vec) const;
	bool collide_rect(const Rect &rect) const;
	// Clamps the rect within the rect passed
	Rect clamp(const Rect &rect) const;
	// Returns if the rect is clamped or not
	bool clamp_ip(const Rect &rect);
	Rect move(const Vector &vec) const;
	void move_ip(const Vector &vec);
};


struct Circle {
	float x, y, r;

	Circle() {};
	Circle(float x, float y, float r);
	Circle(const Vector &vec, const float radius);

	friend std::ostream& operator<<(std::ostream &os, const Circle &circle);

	const string to_str() const;

	float radius() const;
	void radius(const float radius);
	Vector center() const;
	void center(const Vector &vec);

	bool collide_point(const Vector &vec) const;
	bool collide_circle(const Circle &circle) const;
	// Clamps the circle within the circle passed
	Circle clamp(const Circle &circle) const;
	// Returns if the circle is clamped or not
	bool clamp_ip(const Circle &circle);
	Circle move(const Vector &vec) const;
	void move_ip(const Vector &vec);
};


struct EventKey {
	int primary, secondary = SDLK_UNKNOWN;
	bool pressed = false, released = false, down = false;
};

typedef std::unordered_map<string, EventKey> EventKeys;
extern EventKeys EVENT_KEYS;


struct MouseButton {
	uint8_t id;
	bool pressed = false, released = false, down = false;
};


struct Finger {
	SDL_FingerID id;
	Vector pos;
	Vector dpos;
	bool pressed;
	float pressure;
};

typedef std::unordered_map<SDL_FingerID, Finger> Fingers;
extern Fingers FINGERS;



// Classes
class Engine {
	public:
		Engine(
			const unsigned int sdl_init_flags=127537,
			const int img_init_flags=2,
			const int mix_init_flags=24
		);
		~Engine();
};


class Clock {
	private:
		uint64_t current_time, last_tick = 0;
		uint64_t timeit_tick = 0;
		int64_t target_ft, delay;

	public:
		// time: Time used in the previous tick in ms
		// raw_time: Actual time used in the previous tick in ms
		// fps: The average framerate of the last 10 ticks
		uint64_t raw_time = 0, frame_time = 0;
		
		// The parameter target_fps should be 0 for unclamped fps
		double tick(double target_fps=0);
		double get_fps();
		// Returns the time between two of its successive calls
		double timeit();
};


class Timer {
	public:
		double time, counter = 0;

		Timer() {};
		// Time should be in seconds
		Timer(double time);

		// Returns true once after the set time is over and gets reset if the
		// _reset parameter is true
		bool update(double dt, bool _reset=true);
		// Sets the counter to 0
		void reset();

		double time_left();
};


class IO {
	private:
		// Used to detect if the file exists and loaded properly
		bool IS_LOADED = false;

	public:
		SDL_IOStream *io;

		IO(const string &file, const string access_mode="r");
		~IO();

		Sint64 get_file_size();
		// The size parameter takes the size of the object to read in bytes
		// and the max parameter takes the maximum number of objects to read
		// Returns the number of objects read or -1 on error
		int read(void *ptr, const int max);
		// Reads the whole file at once to a string
		string read();
		// Reads the next max number of chars from the file to a string
		string read(const int max);
		// The size parameter takes the size of the object to read in bytes
		// and the num parameter takes the number of objects to write
		// Returns the numer of objects written
		void write(const void *ptr, const size_t num);
		void write(const string &data);
		Sint64 tell();
		// The parameter whence can be any of
		// RW_SEEK_SET, RW_SEEK_CUR or RW_SEEK_END
		Sint64 seek(Sint64 offset, int whence=SDL_IO_SEEK_CUR);
		void close();
};


class Window {
	public:
		managed_ptr<SDL_Window> window;

		Window(
			const string &title,
			const IVector &size,
			const uint32_t flags=0
		);

		void static destroy(SDL_Window *window);
		void wrap_mouse(const Vector &wrap_pos);
		// This function should be only used if the renderer is created with an
		// SDL_RENDERER_SOFTWARE flag
		Surface get_window_surface();
		void gl_swap();
};


class Renderer {
	public:
		managed_ptr<SDL_Renderer> renderer;

		Renderer(
			Window &window,
			const uint32_t flags=0,
			const string &driver=""
		);

		void static destroy(SDL_Renderer *renderer);
		void set_colour(const Colour &colour);
		void clear(const Colour &colour);
		void present();
		void flush();
		void set_blend_mode(const SDL_BlendMode blend_mode);
		void set_target(); // Resets the render target to the window
		void set_target(Texture &tex);
		void set_logical_presentation(
			const IVector &size,
			const SDL_RendererLogicalPresentation
					mode=SDL_LOGICAL_PRESENTATION_DISABLED,
			const SDL_ScaleMode scale_mode=SDL_SCALEMODE_BEST
		);
		IVector get_output_size();
		SDL_RendererInfo get_info();
		string get_driver_name();
		void draw_point_raw(const Vector &point_pos);
		void draw_point(const Vector &point_pos, const Colour &colour);
		void draw_line_raw(const Vector &v1, const Vector &v2);
		void draw_line(
			const Vector &v1,
			const Vector &v2,
			const Colour &colour
		);
		void draw_line(
			const Vector &v1,
			const Vector &v2,
			const Colour &colour,
			const float width
		);
		void draw_rect_raw(const Rect &rect, const float width=0);
		void draw_rect(const Rect &rect,
			const Colour &colour,
			const float width=0
		);
		void draw_circle(const Circle &circle,
			const Colour &colour,
			const bool filled=true
		);
		void draw_polygon(const std::vector<Vector> vertices,
			const Colour colour,
			const bool filled=true
		);
		void render_geometry_raw(const int num_vertices,
			const SDL_Vertex *vertices,
			const int num_indices,
			const int *indices
		);
		void render_geometry_raw(const int num_vertices,
			const SDL_Vertex *vertices,
			const int num_indices,
			const int *indices,
			Texture &texture
		);
		void render_geometry(const std::vector<SDL_Vertex> &vertices);
		void render_geometry(const std::vector<SDL_Vertex> &vertices,
			const std::vector<int> &indices
		);
		void render_geometry(
			const std::vector<SDL_Vertex> &vertices,
			Texture &texture
		);
		void render_geometry(
			const std::vector<SDL_Vertex> &vertices,
			const std::vector<int> &indices,
			Texture &texture
		);
		void render_geometry_sorted(
			const std::vector<SDL_Vertex> &vertices
		);
		void render_geometry_sorted(
			const std::vector<SDL_Vertex> &vertices,
			Texture &texture
		);
};


class Mouse {
	public:
		Vector pos = {0, 0};
		std::unordered_map<int, MouseButton> buttons;
		// The amount scrolled vertically, positive away from the user and
		// negative towards the user
		float vert_wheel = 0;
		// The amount scrolled horizontally, positive to the right and negative
		// to the left
		float horz_wheel = 0;

		Mouse(const int needed_buttons = 0);

		static void set_relative_mode(const bool val);
		// The function calls window.wrap_mouse and updates the mouse position
		// to wrap_pos
		void wrap_in_window(Window &window, const Vector &wrap_pos);
};


class Events {
	public:
		SDL_Event event;
		bool running = true;

		// The function event handler should return true if the engine loop
		// should not be run otherwise false
		bool process_events(
			EventKeys *event_keys = nullptr,
			Mouse *mouse = nullptr,
			Fingers *fingers = nullptr,
			std::function<bool(SDL_Event&)> event_handler = nullptr
		);
};


extern int SURF_ID;
class Surface {
	public:
		int id;
		managed_ptr<SDL_Surface> surface;
		int w, h;

		Surface(
			const IVector &size,
			const SDL_PixelFormatEnum format=SDL_PIXELFORMAT_RGBA8888
		);
		Surface(SDL_Surface *_surface);
		Surface(Surface &&_surface);
		Surface(const string &file);

		void set_blend_mode(const SDL_BlendMode blend_mode);
		void set_colour_key(const uint32_t key, const bool enable=true);
		void flip(const SDL_FlipMode flip_mode);
		void blit(Surface &dst_surface, const IVector &ivec);
		void blit(Surface &dst_surface, const IRect &dst_rect);
		void blit(
			Surface &dst_surface,
			const IRect &dst_rect,
			const IRect &src_rect
		);
		// This function saves the surface as png
		void save(const string &file);
		// This function saves the surface as jpg
		// quality should be between 0 to 100
		void save(const string &file, const int quality);
};


extern int TEX_ID;
class Texture {
	public:
		int id;
		managed_ptr<SDL_Texture> texture;
		Renderer *tex_renderer;
		int w, h;

		Texture(Renderer &renderer, SDL_Texture *_texture);
		Texture(Texture &&_texture);
		Texture(Renderer &renderer, const string &file);
		Texture(Renderer &renderer, const Surface &surface);
		Texture(
			Renderer &renderer,
			const IVector &size,
			const SDL_PixelFormatEnum format=SDL_PIXELFORMAT_RGBA32,
			const int access=SDL_TEXTUREACCESS_TARGET
		);

		IRect get_rect();
		SDL_PropertiesID get_properties();
		int get_gl_texture_number();

		void set_colour_mod(const Colour &colour);
		void set_blend_mode(const SDL_BlendMode blend_mode);
		void render(const Vector &vec);
		void render(const Rect &dst_rect);
		void render(const Rect &dst_rect, const Rect &src_rect);
		void render_rot(
			const Rect &dst_rect,
			const float angle=0,
			const Vector &center={0, 0},
			const SDL_FlipMode flip=SDL_FLIP_NONE
		);
		void render_rot(
			const Rect &dst_rect,
			const Rect &src_rect,
			const float angle=0,
			const Vector &center={0, 0},
			const SDL_FlipMode flip=SDL_FLIP_NONE
		);
};


class Camera {
	private:
		// [-1/0/1] -> rejected/pending/approved
		int permission_state;
		SDL_Surface *surface;
		std::unique_ptr<Surface> frame;

	public:
		managed_ptr<SDL_Camera> camera;
		uint64_t time_stamp = 0;

		Camera(const int id=0);

		static std::vector<SDL_CameraDeviceID> get_available_devices();
		// Throws an error if the ID is greater than the number of available
		// devices
		static SDL_CameraDeviceID select_device(const int id=0);

		int get_permission_state();
		// This function must be called before acquire_frame()
		// Otherwise acquire_frame() will keep returning the old surface
		// Also returns false if camera permission hasn't been granted
		// if a new frame isn't available
		bool is_new_frame_available();
		Surface& acquire_frame();
		void release_frame();
};

#endif /* SUPERNOVA_CORE_H */
