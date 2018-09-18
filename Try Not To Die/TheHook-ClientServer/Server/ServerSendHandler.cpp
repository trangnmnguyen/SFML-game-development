#include "ServerSendHandler.h"

void ServerSendHandler::operator()(std::mutex* mu, int clientId, SOCKET clientSocket, Game* game) const {
	
	// Wait until game start
	while(game->getGameState() == GameState::NOT_STARTED);

	// Send messages describing game status to client every time game is updated
	while(game->getGameState() == GameState::PLAYING) {
		try {
			if(game->hasUpdate()) {
				mu->lock();
				game->unmarkUpdate();
				mu->unlock();
				std::string statusString = game->getStatusString();
				const char* messageBuffer = statusString.c_str();
				int sendStatus = send(clientSocket, messageBuffer, strlen(messageBuffer) + 1, 0);
				if(sendStatus == SOCKET_ERROR) {
					throw SendingError(INVALID_SOCKET);
				}
			}
		}

		catch(std::exception& e) {
			mu->lock();
			std::cerr << e.what() << std::endl;
			std::cout << "Stop sending to client #" << clientId << std::endl;
			mu->unlock();
			return;
		}
	}

	// Send message describing game status to client when game is over
	try {
		const char* messageBuffer = game->getStatusString().c_str();
		send(clientSocket, messageBuffer, strlen(messageBuffer), 0);
	}

	catch(std::exception& e) {
		mu->lock();
		std::cerr << e.what() << std::endl;
		mu->unlock();
	}
}
