#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;

	//Where is the player
	Vector2f m_Position;
	Sprite m_Sprite;
	Texture m_Texture;
	Vector2f m_Resolution;
	IntRect m_Arena;
	int m_TileSize;

	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	int m_Health;
	int m_MaxHealth;
	Time m_LastHit;
	float m_Speed;

public:
	Player();

	//prepare player ready to use, including putting it into its start location
	void spawn(IntRect arena, Vector2f resolution, int tileSize);

	//at the end of every game
	void resetPlayerStats();

	//hanle the player getting hit by a zombie
	bool hit(Time timeHit);

	//return the time when the player was last hit
	Time getLastHitTime();

	//horizontal and vertical cordinates of the person. for collision detection
	FloatRect getPosition();

	//get the center cordinates of the char
	Vector2f getCenter();

	//which angle is the player facing
	float getRotation();

	//send a copy of the sprite to main
	Sprite getSprite();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	//take action when keys have been released
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	//make sure player's data is updated. hold horizontal and vertical screen location of the mouse pointer
	void update(float elapsedTime, Vector2i mousePosition);

	//leveling up the player
	void upgradeSpeed();

	//leveling up 
	void upgradeHealth();

	//when player picks up a health pickup
	void increaseHealthLevel(int amount);

	//determine a player's health at any given moment
	int getHealth();
};