#include "Game.h"

Game::Game() {
	state = GameState::NOT_STARTED;
	isUpdate = false;
	players = new Player*[NUM_PLAYERS];
	for(int playerId = 0; playerId < NUM_PLAYERS; playerId++) {
		players[playerId] = new Player();
	}
}

Game::~Game() {
	for(int playerId = 0; playerId < NUM_PLAYERS; playerId++) {
		delete players[playerId];
	}
	delete[] players;
}

GameState Game::getGameState() const {
	return state;
}

void Game::setGameState(GameState newState) {
	state = newState;
}

bool Game::hasUpdate() const {
	return isUpdate;
}

void Game::markUpdate() {
	isUpdate = true;
}

void Game::unmarkUpdate() {
	isUpdate = false;
}

int Game::getPlayerScore(int playerId) const {
	return players[playerId]->getScore();
}

void Game::setPlayerScore(int playerId, int newScore) {
	players[playerId]->setScore(newScore);
}

void Game::increasePlayerScore(int playerId) {
	players[playerId]->increaseScore();
}

void Game::decreasePlayerScore(int playerId) {
	players[playerId]->decreaseScore();
}
