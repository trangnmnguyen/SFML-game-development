// TheHook.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "TextureHolder.h"
#include "Player.h"
#include <iostream>
#include "Hook.h"
#include <sstream>

using namespace sf;

int main()
{
	TextureHolder holder;
	enum class State {
		PAUSED, GAME_OVER, PLAYING, HOMESCREEN, DEAD
	};

	enum class Side {
		LEFT, RIGHT
	};

	State state = State::HOMESCREEN;

	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y),"Try Not To Die", Style::Fullscreen);

	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	Clock clock;
	Time gameTimeTotal;
	Time lastPressed;

	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;

	Player player1("graphics/player3.png");
	Player player2("graphics/player4.png");

	Hook hook1;
	Hook hook2;

	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	//when did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;

	//how often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;

	Sprite spriteBackground;
	Texture textureBackground = TextureHolder::GetTexture("graphics/playground.png");
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setOrigin(0, 0);

	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/gameover.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setOrigin(0, 0);

	Sprite spriteHomescreenBackground;
	Texture textureHomescreenBackground = TextureHolder::GetTexture("graphics/intro.jpg");
	spriteHomescreenBackground.setTexture(textureHomescreenBackground);
	spriteHomescreenBackground.setTexture(textureHomescreenBackground);
	spriteHomescreenBackground.setOrigin(0, 0);

	Font fontHeader;
	fontHeader.loadFromFile("font/junegull.ttf");
	Font fontPressEnter;
	fontPressEnter.loadFromFile("font/pressEnter.ttf");

	Text theHookText;
	theHookText.setFont(fontHeader);
	theHookText.setCharacterSize(150);
	theHookText.setFillColor(Color::White);
	theHookText.setPosition(480, 200);
	theHookText.setString("Try Not To Die");

	Text pressToPlayText;
	pressToPlayText.setFont(fontPressEnter);
	pressToPlayText.setCharacterSize(80);
	pressToPlayText.setFillColor(Color::White);
	pressToPlayText.setPosition(600, 700);
	pressToPlayText.setString("Press Enter to Play");

	srand((int)time(0));
	int side = (rand() % 2);

	int player1Score = 0;
	Text player1ScoreText;
	player1ScoreText.setFont(fontPressEnter);
	player1ScoreText.setCharacterSize(40);
	player1ScoreText.setFillColor(Color::White);
	
	std::stringstream s1;
	s1 << "Player 1 Score: " << player1Score;
	player1ScoreText.setString(s1.str());

	int player2Score = 0;
	Text player2ScoreText;
	player2ScoreText.setFont(fontPressEnter);
	player2ScoreText.setCharacterSize(40);
	player2ScoreText.setFillColor(Color::White);
	
	std::stringstream s2;
	s2 << "Player 2 Score: " << player2Score;
	player2ScoreText.setString(s2.str());

	if (side == 1) {
		player1ScoreText.setPosition(270, 100);
		player2ScoreText.setPosition(1370, 100);
	}
	else {
		player1ScoreText.setPosition(1370, 100);
		player2ScoreText.setPosition(270, 100);
	}

	while (window.isOpen()) {
		/*
		*******************
		Handle Input
		*******************
		*/

		//Handle events
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Return && state == State::HOMESCREEN) {
					state = State::PLAYING;
					if (state == State::PLAYING) {
						IntRect arena1;
						IntRect arena2;

						arena1.width = 700;
						arena1.height = 1080;
						arena1.left = 150;
						arena1.top = 0;

						arena2.width = 1780;
						arena2.height = 1080;
						arena2.left = 1220;
						arena2.top = 0;

						if (side == 1) {
							player1.spawn(arena1, resolution);
							player2.spawn(arena2, resolution);
						}
						else {
							player1.Rotate(180);
							player2.Rotate(180);
							player1.spawn(arena2, resolution);
							player2.spawn(arena1, resolution);
						}
						mouseScreenPosition = (Vector2i)player1.getCenter();
					}
					clock.restart();
				}
				else if (event.key.code == Keyboard::Return && state == State::PAUSED) {
					state = State::PLAYING;
					clock.restart();
				}
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER) {
					state == State::HOMESCREEN;
					clock.restart();
				}
				else if (event.key.code == Keyboard::Return && state == State::PLAYING) {
					state == State::PAUSED;
				}
			}
		}//end event polling

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
		
		/*
		*****************
		update the frame
		*****************
		*/
		if (state == State::PLAYING) {
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();
			if (Mouse::isButtonPressed(Mouse::Right)) {
				mouseScreenPosition = Mouse::getPosition();
				mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);
				player1.setAngle(mouseScreenPosition);
			}

			if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::Q) {
					if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000) {
						hook1.shoot(player1.getCenter().x, player1.getCenter().y, mouseScreenPosition);
					}
				}
			}
				
			player1.movePlayer(mouseScreenPosition, dtAsSeconds);
			
			if (hook1.isInFlight()) {
				hook1.update(mouseScreenPosition, dtAsSeconds);
			}

			//collision detection 
			if (hook1.isInFlight() && hook1.getPosition().intersects(player2.getPosition())) {
				hook1.stop();
				player1Score++;
				std::stringstream p1;
				p1 << "Player 1 Score: " << player1Score;
				player1ScoreText.setString(p1.str());
				//state = State::GAME_OVER;
			}

			if (hook2.isInFlight() && hook2.getPosition().intersects(player1.getPosition())) {
				hook2.stop();
				player2Score++;
				std::stringstream p2;
				p2 << "Player 1 Score: " << player1Score;
				player1ScoreText.setString(p2.str());
				//state = State::GAME_OVER;
			}

			if (player1Score == 5 || player2Score == 5) state = State::GAME_OVER;
		}

		//player1.Rotate(180);
		/*
		*****************************
		Draw the scene
		*****************************
		*/
		if (state == State::HOMESCREEN) {
			window.clear();
			window.draw(spriteHomescreenBackground);
			window.draw(theHookText);
			window.draw(pressToPlayText);
		}

		if (state == State::PLAYING) {
			window.clear();
			window.setView(mainView);
			window.draw(spriteBackground);
			if (hook1.isInFlight()) {
				window.draw(hook1.getSprite());
			}
			window.draw(player1.getSprite());
			window.draw(player2.getSprite());
			
			window.setView(hudView);
			window.draw(player2ScoreText);
			window.draw(player1ScoreText);
			//window.draw(player3.getSprite());
		}

		if (state == State::GAME_OVER) {
			window.clear();
			window.draw(spriteGameOver);
		}
		window.display();
	}

    return 0;
}

