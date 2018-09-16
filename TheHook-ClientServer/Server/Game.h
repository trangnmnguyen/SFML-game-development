#ifndef GAME_H
#define GAME_H

#include "GameState.h"
#include "Player.h"
#include "Constants.h"

class Game {
private:
	GameState state;
	bool isUpdate;
	Player** players;

public:
	Game();
	~Game();
	GameState getGameState() const;
	void setGameState(GameState newState);
	bool hasUpdate() const;
	void markUpdate();
	void unmarkUpdate();
	int getPlayerScore(int playerId) const;
	void setPlayerScore(int playerId, int newScore);
	void increasePlayerScore(int playerId);
	void decreasePlayerScore(int playerId);
};

#endif