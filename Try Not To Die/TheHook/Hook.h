#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Hook {
private:
	Vector2f m_Position;
	//Vector2f m_strawPosition;
	Sprite m_Sprite;
	bool m_InFlight = false;
	float m_Speed = 700;
	float m_SpeedX;
	float m_SpeedY;

	float m_MaxX;
	float m_MinY;
	float m_MaxY;
	float m_MinX;

public:
	Hook();
	void stop();
	bool isInFlight();
	void shoot(float startX, float startY, Vector2i targetMouse);
	FloatRect getPosition();
	Sprite getSprite();
	void update(Vector2i targetMouse, float elapsedTine);
	Vector2f getCenter();
};
