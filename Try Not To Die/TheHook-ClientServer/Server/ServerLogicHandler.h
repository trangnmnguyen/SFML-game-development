#ifndef SERVER_LOGIC_HANDLER_H
#define SERVER_LOGIC_HANDLER_H

#include "Game.h"
#include "GameState.h"
#include "Constants.h"
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>

class ServerLogicHandler {
public:
	void operator()(std::mutex* mu, Game* game) const;
};

#endif
