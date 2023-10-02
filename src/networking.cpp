#include <cstring>

#include "networking.h"


using namespace std;



// Classes
Packet::Packet(const int maxlen): packet(managed_ptr<UDPpacket>(SDLNet_AllocPacket(maxlen), SDLNet_FreePacket)) {
	this->maxlen = maxlen;

	for_sending = false;
}

Packet::Packet(const int maxlen, IPaddress &destination):
	packet(managed_ptr<UDPpacket>(SDLNet_AllocPacket(maxlen), SDLNet_FreePacket)) {
	this->maxlen = maxlen;

	this->destination = destination;

	for_sending = true;
}

void Packet::clear_data() {
	data.clear();
}

void Packet::set_data(char *val) {
	clear_data();
	_serialized_data = string(val);
	int end = 0;
	for (int i = 0; i < static_cast<int>(_serialized_data.size()); i++) {
		if (_serialized_data[i] == DELIMITER) {
			data.push_back(_serialized_data.substr(end, i - end));
			end = i + 1;
		}
	}
}

char* Packet::get_data() {
	// Every time this function is called the data becomes empty

	_serialized_data = "";
	for (string val: data) {
		_serialized_data += val + DELIMITER;
	}
	clear_data();
	if (static_cast<int>(_serialized_data.size()) > maxlen)
		SDL_LogError(0, "Packet overflow! Data won't be send.");

	return (char *)_serialized_data.c_str();
}

void Packet::ready() {
	// This function shouldn't be called explicitly by the user.

	if (for_sending) {
		packet->address.host = destination.host;
		packet->address.port = destination.port;
		packet->data = (Uint8 *)strdup(get_data());
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
	strcpy(val, packet.get_next_val().c_str());
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
	SDL_Log("UDPSocket created successfully!");
}

UDPSocket::~UDPSocket() {
	SDLNet_UDP_Close(socket);
	SDL_Log("UDPSocket closed successfully!");
}

bool UDPSocket::send(Packet &packet, const int channel) {
	packet.ready();
	if (SDLNet_UDP_Send(socket, channel, packet.packet.get())) {
		return true;
	} else {
		SDL_LogError(0, "Failed to send packet: %s", SDLNet_GetError());
		return false;
	}
}

bool UDPSocket::recv(Packet &packet) {
	int val = SDLNet_UDP_Recv(socket, packet.packet.get());
	if (val == 1) {
		packet.set();
		return true;
	} else if (val == 0)
		return false;
	else
		SDL_LogError(0, "Packet not received: %s", SDLNet_GetError());
	return false;
}


TCPSocket::TCPSocket() {
	SDL_Log("TCPSocket created successfully!");
}

TCPSocket::TCPSocket(IPaddress &ip) {
	if ((socket = SDLNet_TCP_Open(&ip)) == NULL)
		SDL_LogError(0, "Failed to create socket: %s", SDLNet_GetError());
	else
		SDL_Log("TCPSocket created successfully!");
}

TCPSocket::~TCPSocket() {
	SDLNet_TCP_Close(socket);
	SDL_Log("TCPSocket closed successfully!");
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

void TCPSocket::send(const void *buffer, int size) {
	_val = SDLNet_TCP_Send(socket, buffer, size);
	if (_val < 0)
		SDL_LogError(0, "Invalid socket: %s", SDLNet_GetError());
	else if (_val < size)
		SDL_LogWarn(1, "Full data not sent: %s", SDLNet_GetError());
}

void TCPSocket::send(string &data) {
	send(data.data(), data.length() + 1);
}

int TCPSocket::recv(void *buffer, const int size) {
	return SDLNet_TCP_Recv(socket, buffer, size);
}


void NetUtils::resolve_host(IPaddress &IP, const int port, const string host) {
	if (SDLNet_ResolveHost(&IP, host.c_str(), port) < 0)
		SDL_LogError(0, "Failed to resolve host: %s", SDL_GetError());
}

string NetUtils::get_formatted_ipv4_host(const Uint32 host) {
	string formatted_host;
	formatted_host += to_string(host - ((host >> 8) << 8)) + ".";
	formatted_host += to_string((host >> 8) - ((host >> 16) << 8)) + ".";
	formatted_host += to_string((host >> 16) - ((host >> 24) << 8)) + ".";
	formatted_host += to_string(host >> 24);
	return formatted_host;
}
