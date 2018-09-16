#ifndef SERVER_H
#define SERVER_H

#include "Game.h"
#include "Exceptions.h"
#include "Constants.h"
#include "ClientReceiveHandler.h"
#include "ClientSendHandler.h"
#include "GameLogicHandler.h"
#include <winsock.h>
#include <iostream>
#include <thread>
#include <mutex>

class Server {
public:
	void startServer(int serverPort, Game* game) const;
};

#endif