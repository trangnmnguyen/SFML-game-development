// ZombieArena.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
#include <sstream>
#include <fstream>
#include <SFML/Audio.hpp>

using namespace sf;

int main()
{
	TextureHolder holder;
	//the game will always be in one of 4 states
	enum class State {
		PAUSED, LEVELING_UP, GAME_OVER, PLAYING
	};

	//start with gameover state
	State state = State::GAME_OVER;

	//get screen resolution and create an sfml window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);

	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//clock for timing everything
	Clock clock;
	Time gameTimeTotal;

	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;

	Player player;

	IntRect arena;

	VertexArray background;
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = NULL;

	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;

	Time lastPressed;

	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	//about the game
	int score = 0;
	int hiScore = 0;

	//game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);

	//load font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	//pause
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(85);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");

	//game over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(80);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");

	//levelling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(60);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());

	//ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(50);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);

	//score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(50);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	//load the high score from a text file
	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open()) {
		inputFile >> hiScore;
		inputFile.close();
	}

	//Hi SCore
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(50);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	//zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(50);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");

	//Wave number;
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(50);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");

	//Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);

	//when did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;

	//how often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;

	//prepare the failed sound
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_fail.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);

	//prepare the powerup sound
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	Sound splat;
	splat.setBuffer(splatBuffer);

	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);

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
				//pause a game while playing
				if (event.key.code == Keyboard::Return && state == State::PLAYING) {
					state = State::PAUSED;
				}
				else if (event.key.code == Keyboard::Return && state == State::PAUSED) {
					state = State::PLAYING;
					clock.restart();
				}
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER) {
					state = State::LEVELING_UP;
					wave = 0;
					score = 0;
					currentBullet = 0;
					bulletsSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;

					player.resetPlayerStats();
				}

				if (state == State::PLAYING) {
					if (event.key.code == Keyboard::R) {
						if (bulletsSpare >= clipSize) {
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
							reload.play();
						}
						else if (bulletsSpare > 0) {
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
							reload.play();
						}
						else {
							reloadFailed.play();
						}
					}
				}
			}
		} // end event polling

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		//handle controls while playing
		if (state == State::PLAYING) {
			//handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				player.moveUp();
			}
			else {
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S)) {
				player.moveDown();
			}
			else {
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A)) {
				player.moveLeft();
			}
			else {
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D)) {
				player.moveRight();
			}
			else {
				player.stopRight();
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > (1000 / fireRate) && bulletsInClip > 0) {
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);

					currentBullet++;

					if (currentBullet > 99)
						currentBullet = 0;
					lastPressed = gameTimeTotal;
					shoot.play();
					bulletsInClip--;
				}
			}//end fire a bullet
		}//end WASD while playing

		 //handle the levelling up state
		if (state == State::LEVELING_UP) {
			if (event.key.code == Keyboard::Num1) {
				//increase fire rate
				fireRate++;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2) {
				//increase clip sizes
				clipSize += clipSize;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3) {
				//increase health
				player.upgradeHealth();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4) {
				//increase speed
				player.upgradeSpeed();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5) {
				//upgrade health pickup
				healthPickup.upgrade();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6) {
				//upgrade ammo pickup
				ammoPickup.upgrade();
				state = State::PLAYING;
			}

			if (state == State::PLAYING) {
				//increase the wave number
				wave++;
				//prepare the level
				arena.width = 500 * wave;
				arena.height = 500 * wave;
				arena.left = 0;
				arena.top = 0;

				int tileSize = createBackground(background, arena);

				//spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);

				//configure the pickup
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				//create a horde of zombie
				numZombies = 5 * wave;

				//delete the previously allocated memory(if exists)
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				//play the powerup sound
				powerup.play();

				//reset the clock so there isnt a frame jump
				clock.restart();
			}
		}//end levelling up
		 /*
		 *****************
		 update the frame
		 *****************
		 */
		if (state == State::PLAYING) {
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();
			//static function: if we define a function in a class with a static keyword, calling the class func w/o an instance of the class
			//where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();

			//convert mouse position to world coordinates of mainview
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			//set the crosshair to the mouse world location
			spriteCrosshair.setPosition(mouseWorldPosition);

			player.update(dtAsSeconds, Mouse::getPosition());

			//make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			//make the view centre around the player
			mainView.setCenter(player.getCenter());

			//loop through each zombie and update them
			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive()) {
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			//update any bullets that are inflight
			for (int i = 0; i < 100; i++) {
				if (bullets[i].isInFlight())
					bullets[i].update(dtAsSeconds);
			}
			//update the pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			//size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 70));

			//increment the number of frames
			framesSinceLastHUDUpdate++;
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval) {
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;

				//update the ammo text
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());

				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				//update the high score text
				ssHiScore << "Hi Score:" << hiScore;
				waveNumberText.setString(ssWave.str());

				//update the high score text
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				framesSinceLastHUDUpdate = 0;
			}//end HUD update

			 //collision detection
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < numZombies; j++) {
					if (bullets[i].isInFlight() && zombies[j].isAlive()) {
						if (bullets[i].getPosition().intersects(zombies[j].getPosition())) {
							//stop the bullet
							bullets[i].stop();

							//register the hit and see if it was a kill
							if (zombies[j].hit()) {
								score += 10;
								if (score >= hiScore) {
									hiScore = score;
								}

								numZombiesAlive--;

								if (numZombiesAlive == 0) {
									state = State::LEVELING_UP;
								}
							}
							splat.play();
						}
					}
				}
			}//end zombie being shot

			 //have any zombies touches the player
			for (int i = 0; i < numZombies; i++) {
				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive()) {
					if (player.hit(gameTimeTotal)) {
						hit.play();
					}
					if (player.getHealth() < 0) {
						state = State::GAME_OVER;
						std::ofstream outputFile("gamedata/scores.txt");
						outputFile << hiScore;
						outputFile.close();
					}
				}
			}//end player touched

			 //health pickup
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned()) {
				player.increaseHealthLevel(healthPickup.gotIt());
				//play a sound
				pickup.play();
			}

			//ammo pickup
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned()) {
				bulletsSpare += ammoPickup.gotIt();
				pickup.play();
				reload.play();
			}
		}//end updating the scene
		 /*
		 *****************************
		 Draw the scene
		 *****************************
		 */
		if (state == State::PLAYING) {
			window.clear();
			//set the mainview to be displayed in the window
			//draw everything related to it
			window.setView(mainView);

			window.draw(background, &textureBackground);

			for (int i = 0; i < numZombies; i++) {
				window.draw(zombies[i].getSprite());
			}

			for (int i = 0; i < 100; i++) {
				if (bullets[i].isInFlight())
					window.draw(bullets[i].getShape());
			}
			window.draw(player.getSprite());

			if (ammoPickup.isSpawned())
				window.draw(ammoPickup.getSprite());
			if (healthPickup.isSpawned())
				window.draw(healthPickup.getSprite());

			window.draw(spriteCrosshair);

			window.setView(hudView);

			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
		}

		if (state == State::LEVELING_UP) {
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}

		if (state == State::PAUSED) {
			window.draw(pausedText);
		}

		if (state == State::GAME_OVER) {
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}
		window.display();
	}//end game loop

	delete[] zombies;
	return 0;
}

