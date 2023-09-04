#ifndef SUPERNOVA_NETWORKING_H
#define SUPERNOVA_NETWORKING_H

#ifndef NET_ENABLED
#define NET_ENABLED
#endif /* NET_ENABLED */


#include <SDL2/SDL_net.h>

#include "core.h"


using namespace std;



// Forward Declarations
class Packet;
class UDPSocket;
class TCPSocket;
class NetUtils;



// Classes
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


#endif /* SUPERNOVA_NETWORKING_H */