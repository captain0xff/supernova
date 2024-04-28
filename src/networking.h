#ifndef SUPERNOVA_NETWORKING_H
#define SUPERNOVA_NETWORKING_H

#ifndef NET_ENABLED
#define NET_ENABLED
#endif /* NET_ENABLED */


#include <SDL3_net/SDL_net.h>

#include "core.h"



// Forward Declarations
class Address;
class StreamSocket;
class StreamServer;
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
		string get_string();
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


class StreamSocket {
public:
	enum State {
		RESOLVING_ADDRESS,
		CONNECTING,
		READY,
		DEAD,
		DESTROYED
	};

	const uint16_t port;
	const string host;

	State state;

	SDLNet_Address *address;
	SDLNet_StreamSocket *socket;

	StreamSocket(const uint16_t _port, const string &_host);
	StreamSocket(SDLNet_StreamSocket *_socket);
	StreamSocket(const StreamSocket&) = delete;
	~StreamSocket();

	StreamSocket& operator=(StreamSocket&) = delete;

	State& update();

	// Returns the number of bytes read or -1 on error
	int read(void *buffer, const int size);
	int write(const void *buffer, const int size);
	int write(const string &msg);
};


class StreamServer {
public:
	enum State {
		RESOLVING_ADDRESS,
		CREATING_SERVER,
		READY,
		DEAD
	};

	const uint16_t port;
	const string host;

	State state;
	int last_id = 0;
	std::unordered_map<int, StreamSocket> clients;

	SDLNet_Server *server;
	SDLNet_Address *address;

	StreamServer(const uint16_t _port);
	StreamServer(const uint16_t _port, const string &_host);
	StreamServer(const StreamServer&) = delete;
	~StreamServer();

	StreamServer& operator=(const StreamServer&) = delete;

	State& update();

	// Returns nullptr if no new client is available
	StreamSocket* accept_client();
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
