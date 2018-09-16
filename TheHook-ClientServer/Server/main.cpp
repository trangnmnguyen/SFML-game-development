#include "Game.h"
#include "Server.h"
#include "Exceptions.h"

int main(int argc, char** argv) {
	// Read arguments
	int serverPort;
	try {
		if(argc != 2) {
			throw MainArgumentError();
		}
		serverPort = stoi(std::string(argv[1]));
	}
	catch(std::exception& e) {
		std::cerr << "Error: Wrong arguments" << std::endl;
		return 0;
	}

	// Create game and server
	Game* game = new Game();
	Server* server = new Server();

	// Start server
	server->startServer(serverPort, game);

	// Delete game and server
	delete game;
	delete server;

	return 0;
}
