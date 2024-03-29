#include <optional>
#include <variant>

#include <SDL3/SDL_main.h>

#include "../src/core.h"
#include "../src/constants.h"
#include "../src/networking.h"


using namespace std;



class Server {
private:
	enum States {
		NONE = 0,
		STARTED = 2,
		CONNECTED = 4,
		DONE = 8,
		CLEANED = 16
	};

	bool connected = false, done = false, cleaned = true;
	StreamServer server;
	StreamSocket client;
	vector<char> buffer;
	int size;

	void check_for_connection();
	void recv();
	void clean();

public:
	string msg;

	Server(const uint16_t port);

	bool run();
};

Server::Server(const uint16_t port): server(port), buffer(1024) {}

bool Server::run() {
	if (!done and cleaned) {
		if (connected)
			recv();
		else
			check_for_connection();
	}

	if (!cleaned)
		clean();

	return done;
}

void Server::check_for_connection() {
	if (server.is_new_connection_available()) {
		client = server.accept_client();
		connected = true;
	}
}

void Server::recv() {
	size = client.read(buffer.data(), buffer.size());
	if (size > 0) {
		if (string(buffer.data()) != "quit") {
			msg = buffer.data();
			if (client.write(buffer.data(), buffer.size()) < 0) {
				SDL_Log("Error: Failed to write!");
				cleaned = false;
			}
		} else {
			cleaned = false;
		}
	} else if (size < 0) {
		SDL_Log("Error: Failed to read!");
		cleaned = false;
	}
}

void Server::clean() {
	if (client.get_pending_writes() == 0) {
		cleaned = true;
		done = true;
	}
}


class Client {
private:
	enum States {
		NONE = 0,
		STARTED = 2,
		CONNECTED = 4,
		DONE = 8,
		CLEANED = 16
	};

	bool started = false, connected = false, done = false, cleaned = true;
	StreamSocket socket;
	int size;

	void check_address_status();
	void check_connection_status();
	void send();
	void clean();

public:
	string msg;

	Client(const uint16_t port, const string &host="127.0.0.1");

	bool run();
};

Client::Client(const uint16_t port, const string &host): socket(port, host) {}

bool Client::run() {
	if (!started)
		check_address_status();

	if (!done and started and cleaned) {
		if (connected)
			send();
		else
			check_connection_status();
	}

	if (!cleaned)
		clean();

	return done;
}

void Client::check_address_status() {
	switch (socket.get_address_status()) {
		case -1:
			SDL_Log("Error: Failed to resolve server address!");
			done = true;
			break;
		case 1:
			started = true;
			break;
	}
}

void Client::check_connection_status() {
	switch(socket.get_connection_status()) {
		case -1:
			SDL_Log("Error: Failed to connect with server!");
			done = true;
			break;
		case 1:
			connected = true;
			break;
	}
}

void Client::send() {
	if (msg != "") {
		if (socket.write(msg.c_str(), msg.size() + 1) < 0) {
			SDL_Log("Error: Failed to write!");
			cleaned = false;
		}
	}
	if (msg == "quit") {
		cleaned = false;
	}
}

void Client::clean() {
	if (socket.get_pending_writes() == 0) {
		cleaned = true;
		done = true;
	}
}

int main(int argc, char *argv[]) {

	bool is_client;
	if (argc < 2) {
		SDL_Log("Wrong usage!");
		SDL_Log("Usage: main.exe <--server/--client>");
		return 0;
	} else {
		string arg(argv[1]);
		if (arg == "--server")
			is_client = false;
		else if (arg == "--client")
			is_client = true;
		else {
			SDL_Log("Wrong args!");
			SDL_Log("Usage: main.exe <--server/--client>");
			return 0;
		}
	}

	Engine engine;

	Window window("", {800, 600});
	Renderer renderer(window);

	Clock clock;
	Events events;

	bool rng = true;
	double dt;

	optional<variant<Server, Client>> socket;
	if (is_client) {
		socket.emplace(in_place_type<Client>, 3000);
	} else {
		socket.emplace(in_place_type<Server>, 3000);
	}

	bool pressed_enter = false;

	EVENT_KEYS["ENTER"] = {SDLK_RETURN};

	while (rng) {
		dt = clock.tick(60);

		rng = events.process_events(&EVENT_KEYS);

		pressed_enter = EVENT_KEYS["ENTER"].pressed;

		visit([&](auto& socket) {
			rng &= !socket.run();
			if (is_client) {
				if (pressed_enter)
					socket.msg = "cool";
				else
					socket.msg = "";
			} else if (socket.msg != "") {
				SDL_Log("msg: %s", socket.msg.c_str());
				socket.msg = "";
			}
		}, socket.value());

		renderer.clear(WHITE);
		renderer.present();
	}

	return 0;
}
