#ifndef GAME_H
#define GAME_H

#include "GameState.h"
#include "Player.h"
#include "Constants.h"
#include <iostream>

class Game {
private:
	GameState state;
	bool isUpdate;
	int turn;
	Player** players;

public:
	Game();
	~Game();
	GameState getGameState() const;
	bool hasUpdate() const;
	int getTurn() const;
	int getPlayerScore(int playerId) const;
	std::string getStatusString() const;
	void setGameState(GameState newState);
	void unmarkUpdate();
	void increasePlayerScore(int playerId);
	void decreasePlayerScore(int playerId);
	void startGame();
	void proceedTurn();
	void endGame();
};

#endif
