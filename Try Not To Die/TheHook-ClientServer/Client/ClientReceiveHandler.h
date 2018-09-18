#ifndef CLIENT_RECEIVE_HANDLER_H
#define CLIENT_RECEIVE_HANDLER_H

#include "Game.h"
#include "Exceptions.h"
#include "Constants.h"
#include <winsock.h>
#include <mutex>
#include <iostream>

class ClientReceiveHandler {
public:
	void operator()(std::mutex* mu, SOCKET clientSocket, Game* game) const;
};

#endif
