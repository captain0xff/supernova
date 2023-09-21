#include <supernova/core.h>
#include <supernova/networking.h>

using namespace std;


int main(int argc, char **argv) {
	Engine engine;
	
	UDPSocket socket(0);
	IPaddress srvadd;

	NetUtils::resolve_host(srvadd, 3940, "255.255.255.255");

	Packet packet(512, srvadd);
	packet << "data";

	socket.send(packet);

	return 0;
}
