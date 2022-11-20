#ifndef ENGINE_H
#define ENGINE_H


#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
//#include <SDL2/SDL_font.h>


using namespace std;



// Globals
extern double PI;



// Helper functions
double radians(const double angle); // Degree to radians
double degrees(const double angle); // Radians to degrees



// Main functions
void Init();
void Quit();



// Structs
struct Colour {
	Uint8 r, g, b;
	Uint8 a = 255;

	operator SDL_Color() const;
};


struct Vector {
	double x, y;

	friend ostream& operator<<(ostream &os, Vector const &vector);
	friend Vector operator+(const Vector &vec1, const Vector &vec2);
	friend Vector operator-(const Vector &vec1, const Vector &vec2);
	friend Vector operator*(const Vector &vec, const double &val);
	friend void operator+=(Vector &vec1, const Vector &vec2);
	friend void operator-=(Vector &vec1, const Vector &vec2);
	friend void operator*=(Vector &vec1, const Vector &vec2);
	friend void operator/=(Vector &vec1, const Vector &vec2);

	operator SDL_Point() const;
	operator SDL_FPoint() const;

	double magnitude_squared();
	double magnitude();

	Vector normalize();
	void normalize_ip();
	Vector rotate_rad(const double &angle);
	void rotate_rad_ip(const double &angle);
	Vector rotate(const double &angle); // In degrees
	void rotate_ip(const double &angle); // In degrees
	double distance_to(const Vector &vec);
};


struct Rect {
	public:
		int x, y, w, h;

		friend ostream& operator<<(ostream &os, Rect const &rect);

		operator SDL_Rect() const;

		Vector size();
		void size(const Vector &vec);
		void scale(const Vector &vec);
		// Scales the height and width by the given factor
		void scale(const double val);
		double half_width();
		double half_height();
		Vector half_size();

		double top();
		void top(const double &val);
		double bottom();
		void bottom(const double &val);
		double left();
		void left(const double &val);
		double right();
		void right(const double &val);
		double centerx();
		void centerx(const double &val);
		double centery();
		void centery(const double &val);
		Vector topleft();
		void topleft(const Vector &vec);
		Vector topright();
		void topright(const Vector &vec);
		Vector bottomleft();
		void bottomleft(const Vector &vec);
		Vector bottomright();
		void bottomright(const Vector &vec);
		Vector center();
		void center(const Vector &vec);
		Vector midtop();
		void midtop(const Vector &vec);
		Vector midbottom();
		void midbottom(const Vector &vec);
		Vector midleft();
		void midleft(const Vector &vec);
		Vector midright();
		void midright(const Vector &vec);

		bool collide_point(const Vector &vec);
		bool collide_rect(const Rect &rect);
		// Clamps the rect within the rect passed
		Rect clamp(const Rect &rect);
		bool clamp_ip(const Rect &rect);
};


struct EventKey {
	int primary, secondary = SDLK_UNKNOWN;
	bool pressed = false, released = false, down = false;
};


struct MouseButton {
	Uint8 id;
	bool pressed = false, released = false, down = false;
};



// Forward declarations
class Clock;
class Window;
class Renderer;
class Texture;
class Mouse;
class Events;
class Fonts;
class SpriteSheet;
class AnimatedSprite;
class States;
class Packet;
class Socket;
class NetUtils;




// Classes
class Clock {
    private:
        double current_time;
        double last_time;
        double static target_dt;
        double dt;

    public:
        double tick(double target_fps=0);
        double get_fps();
};


class Window {
	public:
		SDL_Window *window;

		Window(string title, int screen_w, int screen_h, Uint32 flags=0, int posx=SDL_WINDOWPOS_CENTERED, int posy=SDL_WINDOWPOS_CENTERED);

		void destroy();
};


class Renderer {
	public:
		SDL_Renderer *renderer;

		Renderer(Window window, int index=-1, Uint32 flags=SDL_RENDERER_ACCELERATED);

		void destroy();
		void set_colour(const Colour &colour);
		void clear(const Colour &colour);
		void present();
		void set_blend_mode(SDL_BlendMode blend_mode);
		void set_target(); // Resets the render target to the window
		void set_target(Texture &tex);
		void draw_point(const Vector &point_pos, const Colour &colour);
		void draw_line(const Vector &v1, const Vector &v2, const Colour &colour);
		void draw_rect(const Rect &rect, const Colour &colour, const int width=0);
		// Currently filled circle with alpha is not properly supported
		void draw_circle(const Vector &center_pos, const int radius, const Colour &colour, const bool filled=true);
		void draw_polygon(const vector<Vector> vertices, const Colour colour, const bool filled=true);
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
};


class Events {
	public:
		void process_events(bool &running, unordered_map<string, EventKey> &event_keys);
		void process_events(bool &running, unordered_map<string, EventKey> &event_keys, Mouse &mouse);
};


extern int TEX_ID;
class Texture {
	public:
		int id;
		SDL_Texture *texture;
		Renderer *tex_renderer;
		int w, h;

		Texture() {};
		Texture(Renderer &renderer, const string &file);
		Texture(Renderer &renderer, SDL_Surface *surface);
		Texture(Renderer &renderer, const Vector &size, const Uint32 format=SDL_PIXELFORMAT_RGBA32, const int access=SDL_TEXTUREACCESS_TARGET);

		Rect get_rect();

		void set_colour_mod(const Colour &colour);
		void set_blend_mode(SDL_BlendMode blend_mode);
		void render(const Rect &dst_rect);
		void render(const Rect &dst_rect, const Rect &src_rect);
		void render_ex(const Rect &dst_rect, const double &angle=0, const Vector &center={0, 0}, const SDL_RendererFlip &flip=SDL_FLIP_NONE);
		void render_ex(const Rect &dst_rect, const Rect &src_rect, const double &angle=0, const Vector &center={0, 0}, const SDL_RendererFlip &flip=SDL_FLIP_NONE);
		void destroy();
};


extern int FONT_ID;
/*
class Font2 {
	public:
		int id;
		Renderer *font_renderer;
		FC_Font *font;

		Font2(Renderer &renderer, const string file, int size, Colour colour, int style);

		Rect get_rect(const string text);
		void render(Rect &dst_rect, string text);
		void destroy();
};
*/


class Font {
	public:
		int id;
		TTF_Font *font;

		struct FontAtlas {
			Texture atlas;
			unordered_map<char, Rect> data;

			void destroy();
		};

		Font() {};
		Font(const string &file, const int size);

		Texture create_text(Renderer &renderer, const string &text, const Colour &colour, const int &quality=0, const bool &wrap_text=false, const Uint32 &wrap_length=0, const Colour &background_colour={0, 0, 0, 0});
		FontAtlas create_atlas(Renderer &renderer, const string chars, const int quality, const Uint8 &alpha);
		// Alpha value is ignored in the colour argument. It should be set during creation of the atlas.
		void draw_text(FontAtlas &font_atlas, const string &text, const Colour &colour, const int &x, const int &y, const double size);
		void destroy();
};


class SpriteSheet {
	public:
		int tile_x, tile_y, tile_w, tile_h, total_tiles;
		Rect src_rect;
		Texture texture;

		SpriteSheet() {};
		SpriteSheet(Renderer &renderer, const string &file, const int &column, const int &row);

		void set_src_rect(const Rect &src_rect);
		void draw_sprite(const Rect &dst_rect, const int &column, const int &row);
		void destroy();
};


class AnimatedSprite: public SpriteSheet {
	public:
		bool loop;
		double animation_speed; // In animation_frames/sec
		double animation_index = 0;

		AnimatedSprite() {};
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

		UDPpacket *packet;
		vector<string> data;

		Packet() {};
		Packet(const int maxlen);
		Packet(const int maxlen, IPaddress &destination);

		void clear_data();
		// This function shouldn't be called explicitly by the user.
		void ready();
		// This function shouldn't be called explicitly by the user.
		void set();
		void destroy();

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


class Socket {
	public:
		UDPsocket socket;

		Socket() {};
		Socket(const int port);

		// Returns true if the packet is sent successfully
		bool send(Packet &packet, const int chanel=-1);
		// Returns true when a packet is coming
		bool recv(Packet &packet);
		void bind(const IPaddress &ip, const int channel=-1);
		void unbind(const int channel);
		void destroy();
};


class NetUtils {
	public:
		void resolve_host(IPaddress &IP, const int port, const string host=string(INADDR_ANY));
};


#endif /* ENGINE_H */
