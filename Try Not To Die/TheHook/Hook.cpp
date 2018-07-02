#include "stdafx.h"
#include "Hook.h"
#include "TextureHolder.h"

Hook::Hook() {
	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/arrow.png"));
	Texture textureHook = TextureHolder::GetTexture("graphics/arrow.png");
	Vector2u hookSize = textureHook.getSize();
	m_Sprite.setOrigin(hookSize.x / 2, hookSize.y / 2);
	m_Sprite.setScale(0.4, 0.4);
}

void Hook::shoot(float startX, float startY, Vector2i targetMouse) {
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;

	float mouseX = targetMouse.x;
	float mouseY = targetMouse.y;

	float gradient = (startX - mouseX) / (startY - mouseY);
	if (gradient < 0)
		gradient *= -1;

	//ratio between horizontal and vertical
	float ratioXY = m_Speed / (1 + gradient);

	//set speed of X and Y
	m_SpeedY = ratioXY;
	m_SpeedX = ratioXY * gradient;

	if (mouseX < startX)
		m_SpeedX *= -1;

	if (mouseY < startY)
		m_SpeedY *= -1;

	float range = 950;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;

	m_Sprite.setPosition(m_Position);

	float angle = (atan2(mouseY - m_Position.y, mouseX - m_Position.x) * 180) / 3.141;
	m_Sprite.setRotation(angle);
}

void Hook::stop() {
	m_InFlight = false;
}

bool Hook::isInFlight() {
	return m_InFlight;
}

FloatRect Hook::getPosition() {
	return m_Sprite.getGlobalBounds();
}

Sprite Hook::getSprite() {
	return m_Sprite;
}

Vector2f Hook::getCenter() {
	return m_Position;
}

void Hook::update(Vector2i targetMouse, float elapsedTime) {
	float mouseX = targetMouse.x;
	float mouseY = targetMouse.y; 

	m_Position.x += m_SpeedX * elapsedTime;
	m_Position.y += m_SpeedY * elapsedTime;

	m_Sprite.setPosition(m_Position);

	if (m_Position.x < m_MinX || m_Position.x > m_MaxX ||
		m_Position.y < m_MinY || m_Position.y > m_MaxX) {
		m_InFlight = false;
	}
}
