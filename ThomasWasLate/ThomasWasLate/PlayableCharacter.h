#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class PlayableCharacter {
protected:
	Sprite m_Sprite;

	//duration of a jump
	float m_JumpDuration;
	bool m_IsJumping;
	bool m_IsFalling;

	bool m_LeftPressed;
	bool m_RightPressed;

	//how long does the jump last so far
	float m_TimeThisJump;

	//player initiates a jump
	bool m_JustJumped = false;
	
private:
	float m_Gravity;

	float m_Speed = 400;

	Vector2f m_Position;

	//where are the characters various body parts
	FloatRect m_Feet;
	FloatRect m_Head;
	FloatRect m_Right;
	FloatRect m_Left;

	Texture m_Texture;
public:
	void spawn(Vector2f startPosition, float gravity);

	bool virtual handleInput() = 0;

	FloatRect getPosition();

	FloatRect getFeet();
	FloatRect getHead();
	FloatRect getRight();
	FloatRect getLeft();

	Sprite getSprite();

	void stopFalling(float position);
	void stopRight(float position);
	void stopLeft(float position);
	void stopJump();

	Vector2f getCenter();

	void update(float elapsedTime);
};