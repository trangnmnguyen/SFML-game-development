#include "stdafx.h"
#include "Engine.h"

void Engine::input() {
	Event event;
	while (m_Window.pollEvent(event)) {
		if (event.type == Event::KeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				m_Window.close();
			}

			if (Keyboard::isKeyPressed(Keyboard::Return)) {
				m_Playing = true;
			}

			//Switch between Thomas and Bob
			if (Keyboard::isKeyPressed(Keyboard::Q)) {
				m_Character1 = !m_Character1;
			}

			if (Keyboard::isKeyPressed(Keyboard::E)) {
				m_SplitScreen = !m_SplitScreen;
			}
		}
	}

	//Handle input to Thomas
	if (m_Thomas.handleInput()) {
		m_SM.playJump();
	}

	if (m_Bob.handleInput()) {
		m_SM.playJump();
	}
}