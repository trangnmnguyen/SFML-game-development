#ifndef CLIENT_H
#define CLIENT_H

#include "Game.h"
#include "Exceptions.h"
#include "Constants.h"
#include "ClientReceiveHandler.h"
#include "ClientSendHandler.h"
#include <winsock.h>
#include <iostream>
#include <thread>
#include <mutex>

class Client {
public:
	void startClient(unsigned long serverIp, int serverPort, Game* game) const;
};

#endif
