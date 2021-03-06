// Timber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <SFML/Audio.hpp>

//when create a class, create in a namespace to distinguish the name of the class from other people's code
using namespace sf;

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
	//create a video mode object with the resolution of the gamer's monitor
	VideoMode vm(1920, 1080);
	
	//create and open a window for the game
	RenderWindow window(vm, "Timber", Style::Fullscreen);

	//create a texture to hold a graphic on the GPU
	Texture textureBackground;

	//load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");

	//create a sprite
	Sprite spriteBackground;

	//attach the sprite to the texture
	spriteBackground.setTexture(textureBackground);

	//set the sprite background to cover the screen
	spriteBackground.setPosition(0, 0);

	//make a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	//make 3 cloud sprites from 1 texture
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);
	
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	//prepare the bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	//is the bee currently moving?
	bool beeActive = false;
	//how fast can the bee fly?
	float beeSpeed = 0.0f;

	//variable to control time itself
	Clock clock;

	//time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	//track whether the game is running
	bool paused = true;

	//draw some text
	int score = 0;
	sf::Text messageText;
	sf::Text scoreText;

	//choose a font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	//set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);

	//assign the actual message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	//make it really big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	//choose a color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	//position the text
	//hold a rectangle with floating point cordinates
	//getLocalBounds(): get the rectangle around messageText
	FloatRect textRect = messageText.getLocalBounds();
	//change the origin to the center of the textrect
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	//prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	//set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}
	
	//prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	//player starts on the left
	side playerSide = side::LEFT;

	//prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	//prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	//line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float	AXE_POSITION_RIGHT = 1075;

	//prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	//prepare the sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	while (window.isOpen()) {
		/*
			Handle the player input
		*/

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyPressed && !paused) {
				acceptInput = true;

				//hide the axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		//Press Escape -> Close window
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();

		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;
			score = 0;
			timeRemaining = 5;

			//make all the branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++) {
				branchPositions[i] = side::NONE;
			}

			//make sure the gravestone is hidden
			spriteRIP.setPosition(675, 2000);

			//move the player into position
			spritePlayer.setPosition(580, 720);
			acceptInput = true;
		}

		if (acceptInput) {
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				playerSide = side::RIGHT;
				score++;

				timeRemaining += (2 / score) + 0.15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(1200, 720);

				updateBranches(score);

				//set log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;

				chop.play();
			}

			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				playerSide = side::LEFT;
				score++;

				timeRemaining += (2 / score) + 0.15;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(580, 720);

				updateBranches(score);

				//set log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;
				acceptInput = false;

				chop.play();
			}
			
		}
			
		/* 
			update the scene
		*/

		if (!paused) {
			//measure time. clock.restart() return time elapsed
			Time dt = clock.restart();

			//subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			//size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f) {
				//pause the game
				paused = true;

				//change the message shown to the player
				messageText.setString("Out of time!!");

				//reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				//change the origin to the center of the textrect
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				outOfTime.play();
			}

			//set up the bee
			if (!beeActive) {
				//how fast is the bee
				srand((int)time(0) * 10);
				//get a random number from 200 to 399. assign the rest to beespeed
				beeSpeed = (rand() % 200) + 200;

				//how high is the bee
				srand((int)time(0) * 10);
				//get a random number between 500 to 999. assign the rest to height
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else {
				//move the bee
				spriteBee.setPosition(
					spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y
				);
				if (spriteBee.getPosition().x < -100)
					beeActive = false;
			}

			//Manage the clouds
			//cloud 1
			if (!cloud1Active) {
				//how fast is the cloud
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				//how high is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else {
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y
				);
				if (spriteCloud1.getPosition().x > 1920)
					cloud1Active = false;
			}

			//cloud 2
			if (!cloud2Active) {
				//how fast is the cloud
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				//how high is the cloud
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else {
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y
				);
				if (spriteCloud2.getPosition().x > 1920)
					cloud2Active = false;
			}

			//cloud 3
			if (!cloud3Active) {
				//how fast is the cloud
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				//how high is the cloud
				srand((int)time(0) * 20);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else {
				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y
				);
				if (spriteCloud3.getPosition().x > 1920)
					cloud3Active = false;
			}

			//update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			for (int i = 0; i < NUM_BRANCHES; i++) {
				float height = i * 150;
				if (branchPositions[i] == side::LEFT) {
					//Move the sprite to the left side
					branches[i].setPosition(610, height);
					//flip  the sprite round the other way
					branches[i].setRotation(180);
				} 
				else if (branchPositions[i] == side::RIGHT) {
					//Move the sprite to the left side
					branches[i].setPosition(1330, height);
					//flip the sprite cordinate (pixed) round the other way as the default is the left
					branches[i].setRotation(0);
				}
				else {
					//hide the branch
					branches[i].setPosition(3000, height);
				}
			}

			if (logActive) {
				spriteLog.setPosition(
					spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds())
				);

				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000) {
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			if (branchPositions[5] == playerSide) {
				//death
				paused = true;
				acceptInput = false;
				
				//draw the gravestone
				spriteRIP.setPosition(525, 760);

				//hide the player
				spritePlayer.setPosition(2000, 660);

				//change the text of the message
				messageText.setString("SQUISHED!!");

				//center it on the screen
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				death.play();
			}
		}

		/*
			draw the scene

		*/

		//clear everything from the last frame
		window.clear();

		//draw the game scene here
		window.draw(spriteBackground);

		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}

		window.draw(spriteTree);

		window.draw(spritePlayer);

		window.draw(spriteAxe);

		window.draw(spriteLog);

		window.draw(spriteRIP);

		window.draw(spriteBee);

		window.draw(scoreText);

		window.draw(timeBar);

		if (paused)
			window.draw(messageText);

		//show everything we drew. scene is drawn at the background ready to be displayed. This is called double buffering
		window.display();
	}

    return 0;
}

void updateBranches(int seed) {
	//move all branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
		branchPositions[j] = branchPositions[j - 1];

	//spawn a new branch at position 0
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;

	case 1:
		branchPositions[0] = side::RIGHT;
		break;

	default:
		branchPositions[0] = side::NONE;
		break;
	}
}