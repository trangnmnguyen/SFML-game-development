#include "stdafx.h"
#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>

using namespace sf;

void Engine::update(float dtAsSeconds) {
	if (m_NewLevelRequired) {
		loadLevel();
	}

	if (m_Playing) {
		//update Thomas & Bob
		m_Thomas.update(dtAsSeconds);
		m_Bob.update(dtAsSeconds);

		//detect collision
		if (detectionCollisions(m_Thomas) && detectionCollisions(m_Bob)) {
			m_NewLevelRequired = true;
		
			//play the reach goal sound
			m_SM.playReachGoal();
		}
		else {
			detectionCollisions(m_Bob);
		}

		if (m_Bob.getFeet().intersects(m_Thomas.getHead())) {
			m_Bob.stopFalling(m_Thomas.getHead().top);
		}
		else if (m_Thomas.getFeet().intersects(m_Bob.getHead())) {
			m_Thomas.stopFalling(m_Bob.getHead().top);
		}

		m_TimeRemaining -= dtAsSeconds;
		if (m_TimeRemaining <= 0) {
			m_LM.setCurrentLevel(m_LM.getCurrentLevel() - 1);
			m_NewLevelRequired = true;
		}
	}//end if playing

	vector<Vector2f>::iterator it;

	for (it = m_FireEmitters.begin(); it != m_FireEmitters.end(); it++) {
		float posX = (*it).x;
		float posY = (*it).y;
		
		FloatRect localRect(posX - 250, posY - 250, 500, 500);

		if (m_Thomas.getPosition().intersects(localRect)) {
			m_SM.playFire(Vector2f(posX, posY), m_Thomas.getCenter());
		}
	}

	if (m_SplitScreen) {
		m_LeftView.setCenter(m_Thomas.getCenter());
		m_RightView.setCenter(m_Bob.getCenter());
	}
	else {
		if (m_Character1) {
			m_MainView.setCenter(m_Thomas.getCenter());
		}
		else {
			m_MainView.setCenter(m_Bob.getCenter());
		}
	}

	m_FramesSinceLastHUDUpdate++;
	if (m_FramesSinceLastHUDUpdate > m_TargetFramesPerHUDUpdate) {
		stringstream ssTime;
		stringstream ssLevel;

		ssTime << (int)m_TimeRemaining;
		m_Hud.setTime(ssTime.str());

		ssLevel << "Level: " << m_LM.getCurrentLevel();
		m_Hud.setLevel(ssLevel.str());

		m_FramesSinceLastHUDUpdate = 0;
	}

	if (m_PS.running()) {
		m_PS.update(dtAsSeconds);
	}
}

 