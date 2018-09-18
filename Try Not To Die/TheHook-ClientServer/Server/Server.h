#ifndef SERVER_H
#define SERVER_H

#include "Game.h"
#include "Exceptions.h"
#include "Constants.h"
#include "ServerReceiveHandler.h"
#include "ServerSendHandler.h"
#include "ServerLogicHandler.h"
#include <winsock.h>
#include <iostream>
#include <thread>
#include <mutex>

class Server {
public:
	void startServer(int serverPort, Game* game) const;
};

#endif
