#ifndef SERVER_SEND_HANDLER_H
#define SERVER_SEND_HANDLER_H

#include "Game.h"
#include "Exceptions.h"
#include <winsock.h>
#include <mutex>
#include <iostream>

class ServerSendHandler {
public:
	void operator()(std::mutex* mu, int clientId, SOCKET clientSocket, Game* game) const;
};

#endif
