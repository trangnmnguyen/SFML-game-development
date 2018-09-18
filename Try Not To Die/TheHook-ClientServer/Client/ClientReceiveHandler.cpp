#include "ClientReceiveHandler.h"

void ClientReceiveHandler::operator()(std::mutex* mu, SOCKET clientSocket, Game* game) const {

	char messageBuffer[MESSAGE_BUFFER_LENGTH];

	// Receive messages describing game status while game is not over
	while(game->getGameState() != GameState::OVER) {
		try {

			// Try receiving from server within RECEIVE_TIMEOUT ms
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
			std::cout << "Server send: " << messageBuffer << std::endl;
			mu->unlock();

			// Update game state using information sent by server
			char* token = strtok(messageBuffer, " ");
			if(token != nullptr) {
				if(strcmp(token, "NOT_STARTED") == 0) {
					mu->lock();
					game->setGameState(GameState::NOT_STARTED);
					mu->unlock();
				}
				else if(strcmp(token, "PLAYING") == 0) {
					mu->lock();
					game->setGameState(GameState::PLAYING);
					mu->unlock();
				}
				else {
					mu->lock();
					game->setGameState(GameState::OVER);
					mu->unlock();
				}
			}
		}

		catch(std::exception& e) {
			mu->lock();
			std::cerr << e.what() << std::endl;
			std::cout << "Stop receiving from server" << std::endl;
			mu->unlock();
			return;
		}
	}
}
