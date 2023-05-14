#ifndef SUPERNOVA_ENGINE_H
#define SUPERNOVA_ENGINE_H


#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

#ifndef __ANDROID__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_mixer.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>
#include <SDL_mixer.h>
#endif


using namespace std;



// Forward Declarations
struct Vector;
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
class Font;
class FontAtlas;
class SpriteSheet;
class AnimatedSprite;
class States;
class Packet;
class Socket;
class NetUtils;



// Helper functions
double radians(const double angle); // Degree to radians
double degrees(const double angle); // Radians to degrees

int randint(const int end); // Generates a random integer b/w 0 to end (0 included and end excluded).
int randint(const int start, const int end); // Generates a random integer b/w start to end (start included and end excluded).



// Templates
template<typename T> using managed_ptr = std::unique_ptr<T, void(*)(T*)>;



// General functions
void start_text_input();



// Structs
struct Colour {
	Uint8 r, g, b;
	Uint8 a = 255;

	operator SDL_Color() const;

	// The values of mod_r, mod_g, mod_b and mod_a should lie b/w 0 to 1
	Colour modulate(const double mod_r=1, const double mod_g=1, const double mod_b=1, const double mod_a=1) const;
};


struct Vector {
	double x, y;

	friend ostream& operator<<(ostream &os, Vector const &vector);
	friend Vector operator+(const Vector &vec1, const Vector &vec2);
	friend Vector operator-(const Vector &vec1, const Vector &vec2);
	friend Vector operator*(const Vector &vec, const double &val);
	friend Vector operator/(const Vector &vec, const double &val);
	friend void operator+=(Vector &vec1, const Vector &vec2);
	friend void operator-=(Vector &vec1, const Vector &vec2);
	friend void operator*=(Vector &vec1, const Vector &vec2);
	friend void operator/=(Vector &vec1, const Vector &vec2);

	operator SDL_Point() const;
	operator SDL_FPoint() const;
	
	const string to_str() const;

	double magnitude_squared() const;
	double magnitude() const;

	Vector normalize() const;
	void normalize_ip();
	Vector rotate_rad(const double &angle) const;
	void rotate_rad_ip(const double &angle);
	Vector rotate(const double &angle) const; // In degrees
	void rotate_ip(const double &angle); // In degrees
	double distance_to(const Vector &vec) const;
	double angle_rad() const;
	double angle() const; // In degrees
	Vector clamp(const Rect &rect) const;
	Vector clamp(const Circle &circle) const;
	void clamp_ip(const Rect &rect);
	void clamp_ip(const Circle &circle);
};


struct Rect {
	int x, y, w, h;

	Rect() {};
	Rect(int x, int y, int w, int h);
	Rect(const Vector &pos, const Vector &size);

	friend ostream& operator<<(ostream &os, Rect const &rect);

	operator SDL_Rect() const;

	const string to_str() const;

	Vector size() const;
	void size(const Vector &vec);
	void scale(const Vector &vec);
	// Scales the height and width by the given factor
	void scale(const double val);
	double half_width() const;
	double half_height() const;
	Vector half_size() const;

	double top() const;
	void top(const double &val);
	double bottom() const;
	void bottom(const double &val);
	double left() const;
	void left(const double &val);
	double right() const;
	void right(const double &val);
	double centerx() const;
	void centerx(const double &val);
	double centery() const;
	void centery(const double &val);
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
	int x, y, r;

	Circle() {};
	Circle(int x, int y, int r);
	Circle(const Vector &vec, const int radius);

	friend ostream& operator<<(ostream &os, const Circle &circle);

	const string to_str() const;

	double radius() const;
	void radius(const double radius);
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

typedef unordered_map<string, EventKey> EventKeys;
extern EventKeys EVENT_KEYS;


struct MouseButton {
	Uint8 id;
	bool pressed = false, released = false, down = false;
};


struct Finger {
	SDL_FingerID id;
	Vector pos;
	Vector dpos;
	bool pressed;
	double pressure;
};

typedef unordered_map<SDL_FingerID, Finger> Fingers;
extern Fingers FINGERS;



// Classes
class Engine {
	public:
		Engine();
		~Engine();
};


class Clock {
	private:
		Uint64 current_time, last_tick = 0;
		Uint64 timeit_tick = 0;
		Sint64 target_ft, delay;

	public:
		// time: Time used in the previous tick in ms
		// raw_time: Actual time used in the previous tick in ms
		// fps: The average framerate of the last 10 ticks
		Uint64 raw_time = 0, frame_time = 0;
		
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

		// Returns true once after the set time is over and gets reset if the _reset parameter is true
		bool update(double dt, bool _reset=true);
		// Sets the counter to 0
		void reset();

		double time_left();
};


class IO {
	public:
		// Used to detect if the file exists and loaded properly
		bool IS_LOADED = false;

		SDL_RWops *io;

		IO(const string &file, const string access_mode="r");
		
		// The size parameter takes the size of the object to read in bytes
		// and the max parameter takes the maximum number of objects to read
		// Returns the number of objects read or -1 on error
		int read(void *ptr, const int max, const int size=1);
		string read(const int max=1024);
		// The size parameter takes the size of the object to read in bytes
		// and the num parameter takes the number of objects to write
		// Returns the numer of objects written
		void write(const void *ptr, const int num, const int size=1);
		void write(const string &data);
		Sint64 tell();
		// The parameter whence can be any of RW_SEEK_SET, RW_SEEK_CUR or RW_SEEK_END
		Sint64 seek(Sint64 offset, int whence=RW_SEEK_CUR);
		void close();
};


class Window {
	public:
		managed_ptr<SDL_Window> window;

		Window(string title, int screen_w, int screen_h, Uint32 flags=0, int posx=SDL_WINDOWPOS_CENTERED, int posy=SDL_WINDOWPOS_CENTERED);

		void static destroy(SDL_Window *window);
		void wrap_mouse(const Vector &wrap_pos);
};


class Renderer {
	public:
		managed_ptr<SDL_Renderer> renderer;

		Renderer(Window &window, int index=-1, Uint32 flags=SDL_RENDERER_ACCELERATED);

		void static destroy(SDL_Renderer *renderer);
		void set_colour(const Colour &colour);
		void clear(const Colour &colour);
		void present();
		void set_blend_mode(SDL_BlendMode blend_mode);
		void set_target(); // Resets the render target to the window
		void set_target(Texture &tex);
		void set_logical_size(const Vector &size);
		Vector get_output_size();
		void draw_point_raw(const Vector &point_pos);
		void draw_point(const Vector &point_pos, const Colour &colour);
		void draw_line_raw(const Vector &v1, const Vector &v2);
		void draw_line(const Vector &v1, const Vector &v2, const Colour &colour);
		void draw_rect_raw(const Rect &rect, const int width=0);
		void draw_rect(const Rect &rect, const Colour &colour, const int width=0);
		void draw_circle(const Circle &circle, const Colour &colour, const bool filled=true);
		void draw_polygon(const vector<Vector> vertices, const Colour colour, const bool filled=true);
		void render_geometry_raw(const int num_vertices, const SDL_Vertex *vertices, const int num_indices, const int *indices);
		void render_geometry_raw(const int num_vertices, const SDL_Vertex *vertices, const int num_indices, const int *indices, Texture &texture);
		void render_geometry(const vector<SDL_Vertex> &vertices);
		void render_geometry(const vector<SDL_Vertex> &vertices, const vector<int> &indices);
		void render_geometry(const vector<SDL_Vertex> &vertices, Texture &texture);
		void render_geometry(const vector<SDL_Vertex> &vertices, const vector<int> &indices, Texture &texture);
		void render_geometry_sorted(const vector<SDL_Vertex> &vertices);
		void render_geometry_sorted(const vector<SDL_Vertex> &vertices, Texture &texture);
};


class Mouse {
	private:
		unordered_map<string, Uint8> BUTTON_MAP = {
			{"LEFT", SDL_BUTTON_LEFT},
			{"RIGHT", SDL_BUTTON_RIGHT},
			{"MIDDLE", SDL_BUTTON_MIDDLE},
			{"X1", SDL_BUTTON_X1},
			{"X2", SDL_BUTTON_X2}
		};

	public:
		Vector pos = {0, 0};
		unordered_map<string, MouseButton> buttons;
		// The amount scrolled vertically, positive away from the user and negative towards the user
		double vert_wheel = 0;
		// The amount scrolled horizontally, positive to the right and negative to the left
		double horz_wheel = 0;

		// The vector should contain LEFT, RIGHT, MIDDLE, X1 or/and X2 in any order
		Mouse(const vector<string> &needed_buttons = {});

		static void set_relative_mode(const bool val);
		// The function calls window.wrap_mouse and updates the mouse position to wrap_pos
		void wrap_in_window(Window &window, const Vector &wrap_pos);
};


class Events {
	public:
		SDL_Event event;
		bool running = true;

		// The function event handler should return true if the engine loop should not be run otherwise false
		bool process_events(EventKeys *event_keys = nullptr, Mouse *mouse = nullptr, Fingers *fingers = nullptr, function<bool(SDL_Event&)> event_handler = nullptr);
};


extern int SURF_ID;
class Surface {
	public:
		int id;
		managed_ptr<SDL_Surface> surface;

		Surface(SDL_Surface *_surface);
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
		Texture(Renderer &renderer, Surface surface);
		Texture(Renderer &renderer, const Vector &size, const Uint32 format=SDL_PIXELFORMAT_RGBA32, const int access=SDL_TEXTUREACCESS_TARGET);

		Rect get_rect();

		void set_colour_mod(const Colour &colour);
		void set_blend_mode(SDL_BlendMode blend_mode);
		void render(const Vector &vec);
		void render(const Rect &dst_rect);
		void render(const Rect &dst_rect, const Rect &src_rect);
		void render_ex(const Rect &dst_rect, const double &angle=0, const Vector &center={0, 0}, const SDL_RendererFlip &flip=SDL_FLIP_NONE);
		void render_ex(const Rect &dst_rect, const Rect &src_rect, const double &angle=0, const Vector &center={0, 0}, const SDL_RendererFlip &flip=SDL_FLIP_NONE);
		// void destroy();
};


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


class SpriteSheet {
	public:
		int tile_x, tile_y, tile_w, tile_h, total_tiles;
		Rect src_rect;
		Texture texture;

		SpriteSheet(Renderer &renderer, const string &file, const int &column, const int &row);

		void set_src_rect(const Rect &src_rect);
		void draw_sprite(const Rect &dst_rect, const int &column, const int &row);
		// void destroy();
};


class AnimatedSprite: public SpriteSheet {
	public:
		bool loop;
		double animation_speed; // In animation_frames/sec
		double animation_index = 0;

		AnimatedSprite(Renderer &renderer, const string &file, const int &column, const int &row, const double animation_speed, bool loop=true);

		// Returns true when the animation has just completed looping once
		bool update(double dt);
		void render(const Rect &dst_rect);
		void render_ex(const Rect &dst_rect, const double &angle=0, const Vector &center={0, 0}, const SDL_RendererFlip &flip=SDL_FLIP_NONE);
};


class States {
	public:
		struct State {
			string name;
			int priority; // Must be a positive integer
			bool active = false;
			bool quick_change = true;
		};

		unordered_map<string, State> states;

		States(vector<State> &states);

		bool is_active(string state);
		void activate(string state);
		void inactivate(string state);
		string get_top_state();
};


class Packet {
	private:
		string _serialized_data;
		string _val;
		int maxlen;
		IPaddress destination;
		// Determines if the packet is used for receiving data
		// or sending.
		bool for_sending;
		
		string get_next_val();
		// Every time this function is called the data becomes empty
		Uint8* get_data();
		void set_data(char *val);

	public:
		char DELIMITER = '|';

		managed_ptr<UDPpacket> packet;
		vector<string> data;

		Packet(const int maxlen);
		Packet(const int maxlen, IPaddress &destination);

		void clear_data();
		// This function shouldn't be called explicitly by the user.
		void ready();
		// This function shouldn't be called explicitly by the user.
		void set();
		// void destroy();

		friend Packet& operator<<(Packet &packet, const string val);
		friend Packet& operator<<(Packet &packet, const char *val);
		friend Packet& operator<<(Packet &packet, const int val);
		friend Packet& operator<<(Packet &packet, const double val);
		friend Packet& operator<<(Packet &packet, const Uint8 val);
		friend Packet& operator<<(Packet &packet, const Colour &colour);
		friend Packet& operator<<(Packet &packet, const Vector &vec);
		friend Packet& operator<<(Packet &packet, const Rect &rect);

		friend Packet& operator>>(Packet &packet, string &val);
		friend Packet& operator>>(Packet &packet, char *val);
		friend Packet& operator>>(Packet &packet, int &val);
		friend Packet& operator>>(Packet &packet, double &val);
		friend Packet& operator>>(Packet &packet, Uint8 &val);
		friend Packet& operator>>(Packet &packet, Colour &colour);
		friend Packet& operator>>(Packet &packet, Vector &vec);
		friend Packet& operator>>(Packet &packet, Rect &rect);
};


class UDPSocket {
	public:
		UDPsocket socket;

		UDPSocket(const int port);
		~UDPSocket();

		// Returns true if the packet is sent successfully
		bool send(Packet &packet, const int chanel=-1);
		// Returns true when a packet is coming
		bool recv(Packet &packet);
		void bind(const IPaddress &ip, const int channel=-1);
		void unbind(const int channel);
		void destroy();
};


class TCPSocket {
	private:
		int _val;
		IPaddress *ip;

	public:
		TCPsocket socket;

		TCPSocket(IPaddress &ip);
		~TCPSocket();

		// Returns true if connection is accepted successfully
		bool accept(TCPSocket &sock);
		// Returns address of the machine connected to the socket
		IPaddress* get_peer_address();
		void send(const void *buffer, int size);
		void send(string &data);
		// Returns number of bytes received
		int recv(void *buffer, const int size);
		void destroy();
};


class NetUtils {
	public:
		void static resolve_host(IPaddress &IP, const int port, const string host="0.0.0.0");
};


class Mixer {
	public:
		void static open_audio_device(int frequency=MIX_DEFAULT_FREQUENCY, Uint16 format=MIX_DEFAULT_FORMAT, int channels=2, int chunksize=2048);
		void static allocate_channels(int channels);
};


extern int MUSIC_ID;
class Music {
	public:
		int id;
		bool is_paused = false;
		managed_ptr<Mix_Music> music;

		Music(const string &file);

		void play(int loop=0);
		// Returns the current volume
		float volume();
		// The value of the parameter volume should be between 0 to 1
		void volume(float volume);
		void pause();
		void resume();
		// Toogles the music playing i.e resumes if paused and vice-versa
		void toggle();
		// void destroy();
};


extern int SOUND_ID;
class Sound {
	public:
		int id, channel;
		bool is_paused;
		managed_ptr<Mix_Chunk> sound;

		// Currently only supports WAV format
		Sound(const string file);

		// Pass -1 to the loop for looping infinitely
		// The first free channel is choosed by default
		void play(int loop=0, int channel=-1);
		// Returns the current volume
		float volume();
		// The value of the parameter volume should be between 0 to 1
		void volume(float volume);
		void pause();
		void resume();
		// Toogles the music playing i.e resumes if paused and vice-versa
		void toggle();
		// void destroy();		
};


#endif /* SUPERNOVA_ENGINE_H */
