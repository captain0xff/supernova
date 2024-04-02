#include <cstring>

#include "networking.h"


using namespace std;



// Classes
Address::Address(): address(nullptr) {}

Address::Address(SDLNet_Address *_address) {
	if (_address != nullptr) {
		address = _address;
		SDLNet_RefAddress(address);
	}
}

Address::Address(Address& _address) {
	address = _address.address;
	SDLNet_RefAddress(address);
}

Address::Address(const string &host): address(SDLNet_ResolveHostname(host.c_str())) {
	if (address == nullptr)
		SDL_LogError(0, "Failed to resolve host: %s", SDL_GetError());
}

Address::~Address() {
	if (address != nullptr) {
		SDLNet_UnrefAddress(address);
		address = nullptr;
	}
}

Address& Address::operator=(Address& _address) {
	if (&_address == this)
		return *this;

	address = _address.address;
	SDLNet_RefAddress(address);

	return *this;
}

int Address::get_status() {
	if (status == 0) {
		status = SDLNet_GetAddressStatus(address);
	}

	return status;
}

void Address::unref() {
	SDLNet_UnrefAddress(address);
}


AddressHandler::AddressHandler() {
	port = -1;
	host = "";
};

AddressHandler::AddressHandler(const uint16_t _port, const string &_host): address(_host) {
	port = _port;
	host = _host;
};

int AddressHandler::get_address_status() {
	// [-1/0/1] -> failed/pending/resolved
	if (address_status <= 0) {
		address_status = address.get_status();

		if (address_status > 0) {
			create_socket();
			address.unref();
		}
	}

	return address_status;
}


StreamServer::StreamServer(const uint16_t port):
	server(managed_ptr<SDLNet_Server>(SDLNet_CreateServer(NULL, port), SDLNet_DestroyServer)) {
	if (server == nullptr)
		SDL_LogError(0, "Failed to create server: %s", SDL_GetError());
}

bool StreamServer::is_new_connection_available() {
	if (client != nullptr)
		SDLNet_DestroyStreamSocket(client);
	SDLNet_AcceptClient(server.get(), &client);

	return (client != nullptr);
}

StreamSocket StreamServer::accept_client() {
	return StreamSocket(client);
	client = nullptr;
}


StreamSocket::StreamSocket():
	socket(managed_ptr<SDLNet_StreamSocket>(nullptr, SDLNet_DestroyStreamSocket)) {}

StreamSocket::StreamSocket(SDLNet_StreamSocket *_socket):
	socket(managed_ptr<SDLNet_StreamSocket>(_socket, SDLNet_DestroyStreamSocket)) {
	if (socket == nullptr)
		SDL_LogError(0, "Invalid stream-socket");
	else {
		address_status = 1;
		connection_status = 1;
	}
}

StreamSocket::StreamSocket(const uint16_t _port, const string &_host):
	AddressHandler(_port, _host),
	socket(nullptr, SDLNet_DestroyStreamSocket) {}

StreamSocket& StreamSocket::operator=(StreamSocket&& stream_socket) {
	if (this == &stream_socket)
		return *this;

	socket = std::move(stream_socket.socket);
	address = stream_socket.address;
	stream_socket.address.address = nullptr;
	port = stream_socket.port;
	host = stream_socket.host;
	connection_status = stream_socket.connection_status;
	address_status = stream_socket.address_status;

	return *this;
}

int StreamSocket::get_connection_status() {
	// [-1/0/1] -> failed/pending/successful
	if ((address_status > 0) && (connection_status <= 0))
		connection_status = SDLNet_GetConnectionStatus(socket.get());

	return connection_status;
}

int StreamSocket::get_pending_writes() {
	// Returns the number of bytes left to send or -1 on error
	return SDLNet_GetStreamSocketPendingWrites(socket.get());
}

int StreamSocket::read(void *buffer, const int size) {
	// Returns the number of bytes read and -1 on error
	return SDLNet_ReadFromStreamSocket(socket.get(), buffer, size);
}

int StreamSocket::write(const void *buffer, const int size) {
	return SDLNet_WriteToStreamSocket(socket.get(), buffer, size);
}

void StreamSocket::create_socket() {
	socket = managed_ptr<SDLNet_StreamSocket>(SDLNet_CreateClient(address.address, port), SDLNet_DestroyStreamSocket);
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
	return packet << to_string(val);
}

Packet& operator<<(Packet &packet, const int val) {
	return packet << to_string(val);
}

Packet& operator<<(Packet &packet, const float val) {
	return packet << to_string(val);
}

Packet& operator<<(Packet &packet, const double val) {
	return packet << to_string(val);
}

Packet& operator<<(Packet &packet, const uint8_t val) {
	return packet << to_string(val);
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


DatagramSocket::DatagramSocket(const uint16_t _port):
	socket(SDLNet_CreateDatagramSocket(NULL, _port), SDLNet_DestroyDatagramSocket) {}

DatagramSocket::DatagramSocket(const uint16_t _port, const string &_host):
	AddressHandler(_port, _host), socket(nullptr, SDLNet_DestroyDatagramSocket) {}

DatagramSocket::~DatagramSocket() {
	SDLNet_DestroyDatagram(datagram);
}

void DatagramSocket::send(const uint16_t port, const Address &address, Packet &packet) {
	res = SDLNet_SendDatagram(
		socket.get(),
		address.address,
		port,
		packet.buffer.c_str(),
		packet.buffer.size()
	);
	packet.clear();

	if (res < 0)
		SDL_LogError(0, "Failed to send packet: %s", SDL_GetError());
}

bool DatagramSocket::recv(Packet &packet) {
	// Returns true when a packet is available
	res = SDLNet_ReceiveDatagram(socket.get(), &datagram);

	if (res == 0 && datagram != nullptr) {
		packet.clear();
		packet.buffer.assign(datagram->buf, datagram->buf + datagram->buflen);

		return true;
	} else if (res < 0) {
		SDL_LogError(0, "Failed to receive packet: %s", SDL_GetError());
	}

	return false;
}

void DatagramSocket::create_socket() {
	socket = managed_ptr<SDLNet_DatagramSocket>(SDLNet_CreateDatagramSocket(address.address, port), SDLNet_DestroyDatagramSocket);
}
