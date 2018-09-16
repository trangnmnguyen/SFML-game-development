#include "ClientReceiveHandler.h"

void ClientReceiveHandler::operator()(std::mutex* mu, int clientId, sockaddr_in clientAddress, int clientAddressLength, SOCKET clientSocket, Game* game) const {
	// Wait until game start
	while(game->getGameState() == GameState::NOT_STARTED);

	// Receive messages of form "+" or "-" from client while game is being played
	while(game->getGameState() == GameState::PLAYING) {
		try {
			char messageBuffer[MESSAGE_BUFFER_LENGTH];
			int receiveStatus = recv(clientSocket, messageBuffer, MESSAGE_BUFFER_LENGTH, 0);
			if(receiveStatus == SOCKET_ERROR) {
				throw ReceivingError(INVALID_SOCKET);
			}
			messageBuffer[receiveStatus] = '\0';
			mu->lock();
			std::cout << "Client " << clientId << " send: " << messageBuffer << std::endl;
			mu->unlock();

			// If client sends "+", increase score, else if client sends "-", decrease score, else do nothing
			if(strcmp(messageBuffer, "+\n") == 0) {
				mu->lock();
				game->increasePlayerScore(clientId);
				mu->unlock();
			}
			else if(strcmp(messageBuffer, "-\n") == 0) {
				mu->lock();
				game->decreasePlayerScore(clientId);
				mu->unlock();
			}
		}
		catch(std::exception& e) {
			mu->lock();
			std::cerr << e.what() << std::endl;
			mu->unlock();
		}
	}
}