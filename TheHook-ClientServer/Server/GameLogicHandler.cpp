#include "GameLogicHandler.h"

// Process game logic: Run game through NUM_TURNS turns, each turn increase all player scores
void GameLogicHandler::operator()(std::mutex* mu, Game* game) const {

	// Initialize game: set game state to GameState::PLAYING and set all player scores to INITIAL_SCORE
	mu->lock();
	game->setGameState(GameState::PLAYING);
	for(int playerId = 0; playerId < NUM_PLAYERS; playerId++) {
		game->setPlayerScore(playerId, INITIAL_SCORE);
	}
	game->markUpdate();
	std::cout << "| Game start ";
	for(int playerId = 0; playerId < NUM_PLAYERS; playerId++) {
		std::cout << "| Player " << playerId << ": " << game->getPlayerScore(playerId) << " ";
	}
	std::cout << "|" << std::endl;
	mu->unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

	// Go through NUM_TURNS turns, each turn increase all scores
	for(int turn = 0; turn < NUM_TURNS; turn++) {
		mu->lock();
		for(int playerId = 0; playerId < NUM_PLAYERS; playerId++) {
			game->increasePlayerScore(playerId);
		}
		game->markUpdate();
		std::cout << "| Turn " << turn << " ";
		for(int playerId = 0; playerId < NUM_PLAYERS; playerId++) {
			std::cout << "| Player " << playerId << ": " << game->getPlayerScore(playerId) << " ";
		}
		std::cout << "|" << std::endl;
		mu->unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
	}

	// End game: set game state to GameState::OVER
	mu->lock();
	game->setGameState(GameState::OVER);
	game->markUpdate();
	std::cout << "| Game over ";
	for(int playerId = 0; playerId < NUM_PLAYERS; playerId++) {
		std::cout << "| Player " << playerId << ": " << game->getPlayerScore(playerId) << " ";
	}
	std::cout << "|" << std::endl;
	mu->unlock();
}
