#ifndef SUPERNOVA_NETWORKING_H
#define SUPERNOVA_NETWORKING_H

#ifndef NET_ENABLED
#define NET_ENABLED
#endif /* NET_ENABLED */


#include <SDL3_net/SDL_net.h>

#include "core.h"


using namespace std;



// Forward Declarations
class Address;
class StreamServer;
class StreamSocket;



// Classes
class StreamServer {
	private:
		SDLNet_StreamSocket *client = nullptr;

	public:
		managed_ptr<SDLNet_Server> server;

		StreamServer(const uint16_t port);

		// This function must be called before calling accept_client
		bool is_new_connection_available();
		// This function should only be called if is_new_connection_available()
		// returns true
		StreamSocket accept_client();
};


class StreamSocket {
	private:
		// [-1/0/1] -> failed/pending/successful
		int connection_status = 0;
		int address_status = 0;

		void create_socket();

	public:
		uint16_t port;
		string host;
		managed_ptr<SDLNet_StreamSocket> socket;
		SDLNet_Address *address;

		StreamSocket();
		StreamSocket(SDLNet_StreamSocket *_socket);
		StreamSocket(const uint16_t _port, const string &_host="127.0.0.1");

		StreamSocket& operator=(StreamSocket&& stream_socket);

		// [-1/0/1] -> failed/pending/resolved
		int get_address_status();
		// [-1/0/1] -> failed/pending/successful
		int get_connection_status();
		// Returns the number of bytes left to send or -1 on error
		int get_pending_writes();

		// Returns the number of bytes read or -1 on error
		int read(void *buffer, const int size);
		int write(const void *buffer, const int size);
};



#endif /* SUPERNOVA_NETWORKING_H */
