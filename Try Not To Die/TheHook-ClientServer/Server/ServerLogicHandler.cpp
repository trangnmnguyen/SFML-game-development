#include "ServerLogicHandler.h"

// Process game logic: Run game through NUM_TURNS turns, each turn increase all player scores
void ServerLogicHandler::operator()(std::mutex* mu, Game* game) const {

	// Initialize game: set game state to GameState::PLAYING and set all player scores to INITIAL_SCORE
	mu->lock();
	game->startGame();
	std::cout << game->getStatusString() << std::endl;
	mu->unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

	// Go through NUM_TURNS turns, each turn increase all scores
	for(int turn = 1; turn <= NUM_TURNS; turn++) {
		mu->lock();
		game->proceedTurn();
		std::cout << game->getStatusString() << std::endl;
		mu->unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
	}

	// End game: set game state to GameState::OVER
	mu->lock();
	game->endGame();
	std::cout << game->getStatusString() << std::endl;
	mu->unlock();
}
