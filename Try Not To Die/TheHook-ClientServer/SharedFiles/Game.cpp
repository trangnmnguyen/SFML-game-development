#include "Game.h"

Game::Game() {
	state = GameState::NOT_STARTED;
	isUpdate = false;
	turn = 0;
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

bool Game::hasUpdate() const {
	return isUpdate;
}

int Game::getTurn() const {
	return turn;
}

int Game::getPlayerScore(int playerId) const {
	return players[playerId]->getScore();
}

std::string Game::getStatusString() const {
	std::string result;
	if(state == GameState::NOT_STARTED) {
		result += "NOT_STARTED";
	}
	else if(state == GameState::PLAYING) {
		result += "PLAYING";
	}
	else {
		result += "OVER";
	}
	result += " " + std::to_string(turn);
	for(int playerId = 0; playerId < NUM_PLAYERS; playerId++) {
		result += " " + std::to_string(players[playerId]->getScore());
	}
	return result;
}

void Game::setGameState(GameState newState) {
	state = newState;
}

void Game::unmarkUpdate() {
	isUpdate = false;
}

void Game::increasePlayerScore(int playerId) {
	players[playerId]->increaseScore();
}

void Game::decreasePlayerScore(int playerId) {
	players[playerId]->decreaseScore();
}

void Game::startGame() {
	state = GameState::PLAYING;
	isUpdate = true;
	turn = 0;
	for(int playerId = 0; playerId < NUM_PLAYERS; playerId++) {
		players[playerId]->setScore(INITIAL_SCORE);
	}
}

void Game::proceedTurn() {
	isUpdate = true;
	turn++;
	for(int playerId = 0; playerId < NUM_PLAYERS; playerId++) {
		players[playerId]->increaseScore();
	}
}

void Game::endGame() {
	state = GameState::OVER;
	isUpdate = true;
}
