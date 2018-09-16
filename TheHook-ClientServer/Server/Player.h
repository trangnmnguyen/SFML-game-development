#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
	int score;

public:
	Player();
	int getScore() const;
	void setScore(int newScore);
	void increaseScore();
	void decreaseScore();
};

#endif
