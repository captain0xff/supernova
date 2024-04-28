#include <SDL3/SDL_main.h>

#include "../src/core.h"
#include "../src/constants.h"
#include "../src/networking.h"
#include "../src/graphics.h"
#include "../src/logging.h"
#include "../src/mixer.h"
#include "../src/font.h"
#include "../src/print.h"


using namespace std;



int main(int, char**) {
	Engine engine;

	bool server_ready = false;

	StreamServer server(3000);
	StreamSocket *client = nullptr;
	StreamSocket *server_sock = nullptr;

	char sbuffer[64], cbuffer[64] = "Hello Boys";

	Clock clock;

	while (1) {
		clock.tick(30);

		if (server.update() == StreamServer::READY && !server_sock) {
			if (!server_ready) {
				client = new StreamSocket(3000, "127.0.0.1");
				print("Created client!");
			}
			server_sock = server.accept_client();
			if (server_sock)
				print("Accepted client!");
			server_ready = true;
		}

		if (server_sock != nullptr) {
			int size = server_sock->read(sbuffer, 64);
			if (size > 0)
				print(sbuffer);
			else if (size < 0) {
				print("Failed to read!");
				break;
			}
		}

		if (client && client->update() == StreamSocket::READY) {
			if (client->write(cbuffer, 64) < 0)
				break;
		}
	}

	delete client;

	return 0;
}

