#include "stdafx.h"
#include "Player.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics/Sprite.hpp>
#include <stdlib.h>
#include <cmath>
#include <iostream>
using namespace sf;

Player::Player(std::string const& filename)
{
	m_Speed = START_SPEED;
	Texture texturePlayer = TextureHolder::GetTexture(filename);
	m_Sprite = Sprite(TextureHolder::GetTexture(filename));
	Vector2u playerSize = texturePlayer.getSize();
	m_Sprite.setOrigin(playerSize.x / 2, playerSize.y / 2);
	m_Sprite.setScale(0.5, 0.5);
}

void Player::spawn(IntRect arena, Vector2f resolution) {
	m_Position.x = (arena.width - arena.left) / 2 + arena.left;
	m_Position.y = arena.height / 2;

	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;

	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
	m_Sprite.setPosition(m_Position.x, m_Position.y);
}

FloatRect Player::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Vector2f Player::getCenter()
{
	return m_Position;
}

float Player::getRotation()
{
	return m_Sprite.getRotation();
}

Sprite Player::getSprite()
{
	return m_Sprite;
}

void Player::movePlayer(Vector2i targetMouse, float elapsedTime) {
	float mouseX = targetMouse.x;
	float mouseY = targetMouse.y;
	float ratioXY = std::abs((mouseX - m_Position.x) / (mouseY - m_Position.y));

	//std::cout << std::isnan(ratioXY) << std::endl;

	if (std::isnan(ratioXY)) {
		m_SpeedX = m_Speed;
		m_SpeedY = 0;
	}
	else if (ratioXY == 0) {
		m_SpeedX = 0;
		m_SpeedY = m_Speed;
	}
	else {
		m_SpeedY = m_Speed / (float)std::sqrt(1 + ratioXY * ratioXY);
		m_SpeedX = ratioXY * m_SpeedY;
	}

	if (!std::isnan(m_SpeedX) && !std::isnan(m_SpeedY)) {
		if (mouseX > m_Position.x) {
			m_Position.x = m_Position.x + m_SpeedX * elapsedTime;
		}

		if (mouseY > m_Position.y) {
			m_Position.y = m_Position.y + m_SpeedY * elapsedTime;
		}

		if (mouseX < m_Position.x) {
			m_Position.x = m_Position.x - m_SpeedX * elapsedTime;
		}

		if (mouseY < m_Position.y) {
			m_Position.y = m_Position.y - m_SpeedY * elapsedTime;
		}

		if (m_Position.x > m_Arena.width) {
			m_Position.x = m_Arena.width;
		}

		if (m_Position.x < m_Arena.left) {
			m_Position.x = m_Arena.left;
		}

		if (m_Position.y > m_Arena.height) {
			m_Position.y = m_Arena.height;
		}

		if (m_Position.y < m_Arena.top) {
			m_Position.y = m_Arena.top;
		}
	}

	m_Sprite.setPosition(m_Position);
}

void Player::setAngle(Vector2i targetMouse) {
	float mouseX = targetMouse.x;
	float mouseY = targetMouse.y;
	float angle = (atan2(mouseY - m_Position.y, mouseX - m_Position.x) * 180) / 3.141;
	m_Sprite.setRotation(angle);
}

void Player::Rotate(float angle) {
	m_Sprite.setRotation(angle);
}
