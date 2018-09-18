#include "Client.h"
#include "Exceptions.h"

int main(int argc, char** argv) {

	// Read arguments
	unsigned long serverIp;
	int serverPort;
	try {
		if(argc != 3) {
			throw MainArgumentError();
		}
		serverIp = inet_addr(argv[1]);
		serverPort = stoi(std::string(argv[2]));
		if(serverIp == INADDR_NONE || serverIp == INADDR_ANY) {
			throw MainArgumentError();
		}
	}
	catch(std::exception& e) {
		std::cerr << "Error: Wrong arguments" << std::endl;
		return 0;
	}

	// Create game and client
	Game* game = new Game();
	Client* client = new Client();

	// Start client
	client->startClient(serverIp, serverPort, game);

	// Delete game and client
	delete game;
	delete client;

	return 0;
}
