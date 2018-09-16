#ifndef GAME_LOGIC_HANDLER_H
#define GAME_LOGIC_HANDLER_H

#include "Game.h"
#include "GameState.h"
#include "Constants.h"
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>

class GameLogicHandler {
public:
	void operator()(std::mutex* mu, Game* game) const;
};

#endif
