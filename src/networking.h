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
class Packet;
class DatagramSocket;



// Classes
class Address {
	public:
		int status;
		SDLNet_Address *address;

		Address();
		Address(SDLNet_Address *_address);
		Address(const string &host);
		Address(Address& _address);
		~Address();

		Address& operator=(Address& _address);

		// [-1/0/1] -> failed/pending/resolved
		int get_status();
		void unref();
};


class AddressHandler {
	protected:
		int address_status = 0;

		virtual void create_socket() {};

	public:
		uint16_t port;
		string host;
		Address address;

		AddressHandler();
		AddressHandler(const uint16_t _port, const string &_host="127.0.0.1");

		// [-1/0/1] -> failed/pending/resolved
		int get_address_status();
};


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


class StreamSocket: public AddressHandler {
	private:
		// [-1/0/1] -> failed/pending/successful
		int connection_status = 0;

		void create_socket();

	public:
		managed_ptr<SDLNet_StreamSocket> socket;

		StreamSocket();
		StreamSocket(SDLNet_StreamSocket *_socket);
		StreamSocket(const uint16_t _port, const string &_host="127.0.0.1");

		StreamSocket& operator=(StreamSocket&& stream_socket);

		// [-1/0/1] -> failed/pending/successful
		int get_connection_status();
		// Returns the number of bytes left to send or -1 on error
		int get_pending_writes();

		// Returns the number of bytes read or -1 on error
		int read(void *buffer, const int size);
		int write(const void *buffer, const int size);
};


// Follows LIFO order for IO
// Try to keep the packet size less than 512bytes
class Packet {
	private:
		string get_last_element();

	public:
		char DELIMETER = '|'; // ASCII unit separater

		string buffer;

		friend Packet& operator<<(Packet &packet, const string &val);
		friend Packet& operator<<(Packet &packet, const char *val);
		friend Packet& operator<<(Packet &packet, const bool val);
		friend Packet& operator<<(Packet &packet, const int val);
		friend Packet& operator<<(Packet &packet, const float val);
		friend Packet& operator<<(Packet &packet, const double val);
		friend Packet& operator<<(Packet &packet, const uint8_t val);
		friend Packet& operator<<(Packet &packet, const Colour &colour);
		friend Packet& operator<<(Packet &packet, const FColour &fcolour);
		friend Packet& operator<<(Packet &packet, const IVector &ivec);
		friend Packet& operator<<(Packet &packet, const Vector &vec);
		friend Packet& operator<<(Packet &packet, const IRect &irect);
		friend Packet& operator<<(Packet &packet, const Rect &rect);
		friend Packet& operator<<(Packet &packet, const Circle &circle);

		friend Packet& operator>>(Packet &packet, string &val);
		friend Packet& operator>>(Packet &packet, char *val);
		friend Packet& operator>>(Packet &packet, bool &val);
		friend Packet& operator>>(Packet &packet, int &val);
		friend Packet& operator>>(Packet &packet, float &val);
		friend Packet& operator>>(Packet &packet, double &val);
		friend Packet& operator>>(Packet &packet, uint8_t &val);
		friend Packet& operator>>(Packet &packet, Colour &colour);
		friend Packet& operator>>(Packet &packet, FColour &fcolour);
		friend Packet& operator>>(Packet &packet, IVector &ivec);
		friend Packet& operator>>(Packet &packet, Vector &vec);
		friend Packet& operator>>(Packet &packet, IRect &irect);
		friend Packet& operator>>(Packet &packet, Rect &rect);
		friend Packet& operator>>(Packet &packet, Circle &circle);

		void clear();
};

class DatagramSocket: public AddressHandler {
	int res;

	void create_socket();

	public:
		SDLNet_Datagram *datagram;
		managed_ptr<SDLNet_DatagramSocket> socket;

		DatagramSocket(const uint16_t _port);
		DatagramSocket(const uint16_t _port, const string &_host);
		~DatagramSocket();

		void send(const uint16_t port, const Address &address, Packet &packet);
		// Returns true when a packet is available
		bool recv(Packet &packet);
};

#endif /* SUPERNOVA_NETWORKING_H */
