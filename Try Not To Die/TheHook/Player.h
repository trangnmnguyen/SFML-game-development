#pragma once
#include <SFML/Graphics.hpp>;

using namespace sf;

class Player {
private:
	const float START_SPEED = 300;

	Vector2f m_Position;
	Sprite m_Sprite;
	Texture m_Texture;
	Vector2f m_Resolution;
	IntRect m_Arena;
	bool m_ArenaSide;

	Time m_LastHit;
	float m_Speed;
	float m_SpeedX;
	float m_SpeedY;
	int isAlive = true;

public:
	Player(std::string const& filename);

	void spawn(IntRect arena, Vector2f resolution);

	//void resetPlayerStats();

	FloatRect getPosition();

	Vector2f getCenter();

	float getRotation();

	Sprite getSprite();

	void movePlayer(Vector2i targetMouse, float elapsedTime);

	void setAngle(Vector2i targetMouse);

	void Rotate(float angle);
};