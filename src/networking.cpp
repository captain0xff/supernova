#include "networking.h"

#include "logging.h"



// Classes
string NetUtils::get_address_string(SDLNet_Address *address) {
	return string(SDLNet_GetAddressString(address));
}

std::vector<string> NetUtils::get_local_addresses() {
	int num;
	SDLNet_Address **addresses = SDLNet_GetLocalAddresses(&num);

	std::vector<string> address_strings(num);
	for (int i = 0; i < num; i++) {
		address_strings[i] = get_address_string(addresses[i]);
	}
	SDLNet_FreeLocalAddresses(addresses);

	return address_strings;
}


StreamSocket::StreamSocket(const uint16_t _port, const string &_host): port(_port), host(_host) {
	address = SDLNet_ResolveHostname(host.c_str());
	state = RESOLVING_ADDRESS;
}

StreamSocket::StreamSocket(SDLNet_StreamSocket *_socket): port(0) {
	socket = _socket;
	address = SDLNet_GetStreamSocketAddress(socket);

	state = READY;
}

StreamSocket::~StreamSocket() {
	if (state != DESTROYED) {
		SDLNet_WaitUntilStreamSocketDrained(socket, -1);
		SDLNet_DestroyStreamSocket(socket);
		if (address != nullptr)
			SDLNet_UnrefAddress(address); 
		state = DESTROYED;
	}
}

StreamSocket::State& StreamSocket::get_state() {
	switch (state) {
		case RESOLVING_ADDRESS:
			switch (SDLNet_GetAddressStatus(address)) {
				case -1:
					flog_error("Failed to resolve address: {}", SDL_GetError());
					state = DEAD;
					break;
				case 1:
					socket = SDLNet_CreateClient(address, port);
					SDLNet_UnrefAddress(address);
					address = nullptr;
					if (socket == nullptr) {
						flog_error("Failed to create stream socket: {}", SDL_GetError());
						state = DEAD;
					} else {
						flog_error("Stream socket created successfully!");
						state = CONNECTING;
					}
					break;
			}
			break;
		case CONNECTING:
			switch(SDLNet_GetConnectionStatus(socket)) {
				case -1:
					flog_error("Failed to connect with server: {}", SDL_GetError());
					state = DEAD;
					break;
				case 1:
					state = READY;
					break;
			}
			break;
		case DEAD:
			if (SDLNet_GetStreamSocketPendingWrites(socket) <= 0) {
				SDLNet_DestroyStreamSocket(socket);
				state = DESTROYED;
			}
			break;
		default:
			break;
	};

	return state;
}

int StreamSocket::read(void *buffer, const int size) {
	// Returns the number of bytes read and -1 on error
	return SDLNet_ReadFromStreamSocket(socket, buffer, size);
}

int StreamSocket::write(const void *buffer, const int size) {
	return SDLNet_WriteToStreamSocket(socket, buffer, size);
}

int StreamSocket::write(const string &msg) {
	// Sends a null-terminated string
	return write(msg.c_str(), msg.size() + 1);
}


StreamServer::StreamServer(const uint16_t _port): port(_port) {
	address = nullptr;
	state = CREATING_SERVER;
}

StreamServer::StreamServer(const uint16_t _port, const string &_host): port(_port), host(_host) {
	address = SDLNet_ResolveHostname(host.c_str());
	state = RESOLVING_ADDRESS;
}

StreamServer::~StreamServer() {
	SDLNet_DestroyServer(server);
	if (address != nullptr)
		SDLNet_UnrefAddress(address);
}

StreamServer::State& StreamServer::get_state() {
	switch (state) {
		case RESOLVING_ADDRESS:
			switch (SDLNet_GetAddressStatus(address)) {
				case -1:
					flog_error("Failed to resolve server address: {}", SDL_GetError());
					state = DEAD;
					break;
				case 1:
					state = CREATING_SERVER;
					break;
			}
			break;
		case CREATING_SERVER:
			if (address == nullptr) {
				server = SDLNet_CreateServer(NULL, port);
			} else {
				server = SDLNet_CreateServer(address, port);
				SDLNet_UnrefAddress(address);
				address = nullptr;
			}
			if (server == nullptr) {
				flog_error("Failed to create stream server: {}", SDL_GetError());
				state = DEAD;
			} else {
				flog_info("Stream server created successfully!");
				state = READY;
			}
			break;
		default:
			break;
	};

	return state;
}

StreamSocket* StreamServer::accept_client() {
	// Returns nullptr if no new client is available
	SDLNet_StreamSocket *client;

	SDLNet_AcceptClient(server, &client);
	if (client != NULL) {
		auto res = clients.try_emplace(1, client);
		if (res.second)
			return &res.first->second;
	}

	return nullptr;
}


string Packet::get_last_element() {
	size_t len = buffer.size(); 
	size_t end = len - 1;
	while ((buffer[end - 1] != DELIMETER) && (end > 0)) {
		end--;
	}

	string sstr = buffer.substr(end, len - end - 1);
	buffer.resize(end);
	return sstr;
}

Packet& operator<<(Packet &packet, const string &val) {
	packet.buffer += val + packet.DELIMETER;
	return packet;
}

Packet& operator<<(Packet &packet, const char *val) {
	return packet << string(val);
}

Packet& operator<<(Packet &packet, const bool val) {
	return packet << std::to_string(val);
}

Packet& operator<<(Packet &packet, const int val) {
	return packet << std::to_string(val);
}

Packet& operator<<(Packet &packet, const float val) {
	return packet << std::to_string(val);
}

Packet& operator<<(Packet &packet, const double val) {
	return packet << std::to_string(val);
}

Packet& operator<<(Packet &packet, const uint8_t val) {
	return packet << std::to_string(val);
}

Packet& operator<<(Packet &packet, const Colour &colour) {
	return packet << colour.r << colour.g << colour.b << colour.a;
}

Packet& operator<<(Packet &packet, const FColour &fcolour) {
	return packet << fcolour.r << fcolour.g << fcolour.b << fcolour.a;
}

Packet& operator<<(Packet &packet, const IVector &ivec) {
	return packet << ivec.x << ivec.y;
}

Packet& operator<<(Packet &packet, const Vector &vec) {
	return packet << vec.x << vec.y;
}

Packet& operator<<(Packet &packet, const IRect &irect) {
	return packet << irect.x << irect.y << irect.w << irect.h;
}

Packet& operator<<(Packet &packet, const Rect &rect) {
	return packet << rect.x << rect.y << rect.w << rect.h;
}

Packet& operator<<(Packet &packet, const Circle &circle) {
	return packet << circle.x << circle.y << circle.r;
}

Packet& operator>>(Packet &packet, string &val) {
	val = packet.get_last_element();
	return packet;
}

Packet& operator>>(Packet &packet, char *val) {
	strcpy(val, packet.get_last_element().c_str());
	return packet;
}

Packet& operator>>(Packet &packet, bool &val) {
	val = stoi(packet.get_last_element());
	return packet;
}

Packet& operator>>(Packet &packet, int &val) {
	val = stoi(packet.get_last_element());
	return packet;
}

Packet& operator>>(Packet &packet, float &val) {
	val = stof(packet.get_last_element());
	return packet;
}

Packet& operator>>(Packet &packet, double &val) {
	val = stod(packet.get_last_element());
	return packet;
}

Packet& operator>>(Packet &packet, uint8_t &val) {
	val = packet.get_last_element()[0];
	return packet;
}

Packet& operator>>(Packet &packet, Colour &colour) {
	return packet >> colour.a >> colour.b >> colour.g >> colour.r;
}

Packet& operator>>(Packet &packet, FColour &fcolour) {
	return packet >> fcolour.a >> fcolour.b >> fcolour.g >> fcolour.r;
}

Packet& operator>>(Packet &packet, IVector &ivec) {
	return packet >> ivec.y >> ivec.x;
}

Packet& operator>>(Packet &packet, Vector &vec) {
	return packet >> vec.y >> vec.x;
}

Packet& operator>>(Packet &packet, IRect &irect) {
	return packet >> irect.h >> irect.w >> irect.y >> irect.x;
}

Packet& operator>>(Packet &packet, Rect &rect) {
	return packet >> rect.h >> rect.w >> rect.y >> rect.x;
}

Packet& operator>>(Packet &packet, Circle &circle) {
	return packet >> circle.r >> circle.y >> circle.x;
}

void Packet::clear() {
	buffer.clear();
}


Datagram::Datagram(const uint16_t _port, const string _host, Packet &_packet):
	port(_port), host(_host), packet(_packet) {
	address = SDLNet_ResolveHostname(host.c_str());
	state = RESOLVING;
}

Datagram::~Datagram() {
	SDLNet_UnrefAddress(address);
}

Datagram::State& Datagram::get_state() {
	switch (state) {
		case RESOLVING:
			switch (SDLNet_GetAddressStatus(address)) {
				case -1:
					flog_error("Failed to resolve address: {}", SDL_GetError());
					state = DEAD;
					break;
				case 1:
					state = READY;
					break;
			}
		default:
			break;
	};

	return state;
}


// DatagramSocket::DatagramSocket(const uint16_t _port): port(_port) {
// 	address = nullptr;
// 	state = CREATING_SOCKET;
// }

DatagramSocket::DatagramSocket(const uint16_t _port, const string &_host): port(_port), host(_host) {
	address = SDLNet_ResolveHostname(host.c_str());
	state = RESOLVING_ADDRESS;
}

DatagramSocket::~DatagramSocket() {
	SDLNet_DestroyDatagram(datagram);
	SDLNet_DestroyDatagramSocket(socket);
	if (address != nullptr)
		SDLNet_UnrefAddress(address);
}

DatagramSocket::State& DatagramSocket::get_state() {
	switch (state) {
		case RESOLVING_ADDRESS:
			switch (SDLNet_GetAddressStatus(address)) {
				case -1:
					flog_error("Failed to resolve address: {}", SDL_GetError());
					state = DEAD;
					break;
				case 1:
					state = CREATING_SOCKET;
					break;
			}
			break;
		case CREATING_SOCKET:
			if (address == nullptr) {
				socket = SDLNet_CreateDatagramSocket(NULL, port);
			} else {
				socket = SDLNet_CreateDatagramSocket(address, port);
				SDLNet_UnrefAddress(address);
				address = nullptr;
			}
			if (socket == nullptr) {
				flog_error("Failed to create datagram socket: {}", SDL_GetError());
				state = DEAD;
			} else {
				flog_error("Datagram socket created successfully!");
				state = READY;
			}
			break;
		default:
			break;
	};

	return state;
}

void DatagramSocket::send(Datagram &_datagram) {
	int res = SDLNet_SendDatagram(
		socket,
		_datagram.address,
		_datagram.port,
		_datagram.packet.buffer.c_str(),
		_datagram.packet.buffer.size()
	);
	_datagram.packet.clear();

	if (res < 0)
		flog_error("Failed to send packet: {}", SDL_GetError());
}

bool DatagramSocket::recv(Packet &packet) {
	// Returns true when a packet is available
	int res = SDLNet_ReceiveDatagram(socket, &datagram);

	if (res == 0 && datagram != nullptr) {
		packet.buffer.assign(datagram->buf, datagram->buf + datagram->buflen);

		return true;
	} else if (res < 0) {
		flog_error("Failed to receive packet: {}", SDL_GetError());
	}

	return false;
}
