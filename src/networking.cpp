#include <cstring>

#include "networking.h"


using namespace std;



// Classes
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
	socket(managed_ptr<SDLNet_StreamSocket>(nullptr, SDLNet_DestroyStreamSocket)),
	address(nullptr) {}

StreamSocket::StreamSocket(SDLNet_StreamSocket *_socket):
	socket(managed_ptr<SDLNet_StreamSocket>(_socket, SDLNet_DestroyStreamSocket)),
	address(nullptr) {
	if (socket == nullptr)
		SDL_LogError(0, "Invalid stream-socket");
	else {
		address_status = 1;
		connection_status = 1;
	}
}

StreamSocket::StreamSocket(const uint16_t _port, const string &_host):
	socket(managed_ptr<SDLNet_StreamSocket>(nullptr, SDLNet_DestroyStreamSocket)),
	address(SDLNet_ResolveHostname(host.c_str())) {
	port = _port;
	host = _host;
	if (address == nullptr)
		SDL_LogError(0, "Failed to resolve host: %s", SDL_GetError());
}

StreamSocket& StreamSocket::operator=(StreamSocket&& stream_socket) {
	if (this == &stream_socket)
		return *this;

	socket = std::move(stream_socket.socket);
	address = stream_socket.address;
	stream_socket.address = nullptr;
	port = stream_socket.port;
	host = stream_socket.host;
	connection_status = stream_socket.connection_status;
	address_status = stream_socket.address_status;

	return *this;
}

int StreamSocket::get_address_status() {
	if (address_status <= 0) {
		address_status = SDLNet_GetAddressStatus(address);

		if (address_status > 0) {
			create_socket();
			SDLNet_UnrefAddress(address);
		}
	}

	return address_status;
}

int StreamSocket::get_connection_status() {
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
	socket = managed_ptr<SDLNet_StreamSocket>(SDLNet_CreateClient(address, port), SDLNet_DestroyStreamSocket);
}
