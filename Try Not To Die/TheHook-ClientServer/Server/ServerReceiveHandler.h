#ifndef SERVER_RECEIVE_HANDLER_H
#define SERVER_RECEIVE_HANDLER_H

#include "Game.h"
#include "Exceptions.h"
#include "Constants.h"
#include <winsock.h>
#include <mutex>
#include <iostream>

class ServerReceiveHandler {
public:
	void operator()(std::mutex* mu, int clientId, SOCKET clientSocket, Game* game) const;
};

#endif
