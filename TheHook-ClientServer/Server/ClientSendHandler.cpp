#include "ClientSendHandler.h"

void ClientSendHandler::operator()(std::mutex* mu, int clientId, sockaddr_in clientAddress, int clientAddressLength, SOCKET clientSocket, Game* game) const {
	
	// Wait until game start
	while(game->getGameState() == GameState::NOT_STARTED);

	// Send messages of form "PLAYING <Player[0].score> <Player[1].score>" to client every time game is updated
	while(game->getGameState() == GameState::PLAYING) {
		try {
			if(game->hasUpdate()) {
				mu->lock();
				game->unmarkUpdate();
				mu->unlock();
				std::string message("PLAYING");
				for(int playerId = 0; playerId < NUM_PLAYERS; playerId++) {
					message += " " + std::to_string(game->getPlayerScore(playerId));
				}
				const char* messageBuffer = message.c_str();
				int sendStatus = send(clientSocket, messageBuffer, strlen(messageBuffer), 0);
				if(sendStatus == SOCKET_ERROR) {
					throw SendingError(INVALID_SOCKET);
				}
			}
		} catch(std::exception& e) {
			mu->lock();
			std::cerr << e.what() << std::endl;
			mu->unlock();
		}
	}

	// Send message "OVER" to client when game is over
	try {
		const char* messageBuffer = "OVER";
		send(clientSocket, messageBuffer, strlen(messageBuffer), 0);
	}
	catch(std::exception& e) {
		mu->lock();
		std::cerr << e.what() << std::endl;
		mu->unlock();
	}
}