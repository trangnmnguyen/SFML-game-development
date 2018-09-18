#include "ClientSendHandler.h"

void ClientSendHandler::operator()(std::mutex* mu, SOCKET clientSocket, Game* game) const {
	
	// Wait until game start
	while(game->getGameState() == GameState::NOT_STARTED);

	// Send user input messages to server while game is being played
	while(game->getGameState() == GameState::PLAYING) {
		try {
			std::string inputString;
			std::getline(std::cin, inputString);
			const char* messageBuffer = inputString.c_str();
			int sendStatus = send(clientSocket, messageBuffer, strlen(messageBuffer) + 1, 0);
			if(sendStatus == SOCKET_ERROR) {
				throw SendingError(INVALID_SOCKET);
			}
		}

		catch(std::exception& e) {
			mu->lock();
			std::cerr << e.what() << std::endl;
			std::cout << "Stop sending to server" << std::endl;
			mu->unlock();
			return;
		}
	}
}
