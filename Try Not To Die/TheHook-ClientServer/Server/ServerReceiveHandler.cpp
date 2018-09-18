#include "ServerReceiveHandler.h"

void ServerReceiveHandler::operator()(std::mutex* mu, int clientId, SOCKET clientSocket, Game* game) const {
	
	// Wait until game start
	while(game->getGameState() == GameState::NOT_STARTED);

	char messageBuffer[MESSAGE_BUFFER_LENGTH];

	// Receive messages of form "+" or "-" from client while game is being played
	while(game->getGameState() == GameState::PLAYING) {
		try {

			// Try receiving from client within RECEIVE_TIMEOUT ms
			int receiveStatus = recv(clientSocket, messageBuffer, MESSAGE_BUFFER_LENGTH, 0);
			if(receiveStatus == SOCKET_ERROR) {
				if(WSAGetLastError() == WSAETIMEDOUT) {
					continue;
				}
				else {
					throw ReceivingError(INVALID_SOCKET);
				}
			}
			mu->lock();
			std::cout << "Client " << clientId << " send: " << messageBuffer << std::endl;
			mu->unlock();

			// If client sends "+", increase score, else if client sends "-", decrease score, else do nothing
			if(strcmp(messageBuffer, "+") == 0) {
				mu->lock();
				game->increasePlayerScore(clientId);
				mu->unlock();
			}
			else if(strcmp(messageBuffer, "-") == 0) {
				mu->lock();
				game->decreasePlayerScore(clientId);
				mu->unlock();
			}
		}

		catch(std::exception& e) {
			mu->lock();
			std::cerr << e.what() << std::endl;
			std::cout << "Stop receiving from client #" << clientId << std::endl;
			mu->unlock();
			return;
		}
	}
}
