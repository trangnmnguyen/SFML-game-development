#include "Player.h"

Player::Player() {
	this->score = 0;
}

int Player::getScore() const {
	return score;
}

void Player::setScore(int newScore) {
	score = newScore;
}

void Player::increaseScore() {
	score++;
}

void Player::decreaseScore() {
	score--;
}
