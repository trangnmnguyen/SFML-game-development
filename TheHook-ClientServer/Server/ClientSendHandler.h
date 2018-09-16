#ifndef CLIENT_SEND_HANDLER_H
#define CLIENT_SEND_HANDLER_H

#include "Game.h"
#include "Exceptions.h"
#include <winsock.h>
#include <mutex>
#include <iostream>

class ClientSendHandler {
public:
	void operator()(std::mutex* mu, int clientId, sockaddr_in clientAddress, int clientAddressLength, SOCKET clientSocket, Game* game) const;
};

#endif