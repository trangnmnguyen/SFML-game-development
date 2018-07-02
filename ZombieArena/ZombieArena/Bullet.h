#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet {
private:
	Vector2f m_Position;
	RectangleShape m_BulletShape;
	//is this bullet currently whizzing through the air
	bool m_InFlight = false;
	float m_BulletSpeed = 1000;

	//what fraction of 1 pixel does the bullet travel horizontal and vertically each frame?
	float m_BulletDistanceX;
	float m_BulletDistanceY;

	float m_MaxX;
	float m_MinY;
	float m_MaxY;
	float m_MinX;

public:
	Bullet();
	void stop();
	bool isInFlight();
	void shoot(float startX, float startY, float xTarget, float yTarget);
	FloatRect getPosition();
	RectangleShape getShape();
	void update(float elapsedTime);
};