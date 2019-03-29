#include <iostream>
#include "Player.h"
#include <SFML/Graphics.hpp>
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickups.h"
#include <fstream>
#include <sstream>
#include <SFML/Audio.hpp>

using namespace sf;

enum class State 
{
	PAUSED, LEVELING_UP, GAME_OVER, PLAYING
};

int main() {
	
	int numZombies;
	int numZombiesAlive;
	TextureHolder holder;
	Zombie* zombies = nullptr;
	State state = State::GAME_OVER;
	Vector2f resolution(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);
	VideoMode vm(resolution.x,resolution.y);
	RenderWindow window(vm, "ZombieArena",Style::Fullscreen);
	Event ev;
	View mainView(FloatRect(0, 0, resolution.x, resolution.y));
	Clock clock;
	Time gameTimeTotal;
	Vector2f mouseWorldPosition;
	Vector2f mouseScreenPosition;
	VertexArray background;
	Player player;
	IntRect arena;
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");
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
	Pickups healtPickup(PICKUPTYPE::Health);
	Pickups ammoPickup(PICKUPTYPE::Ammo);
	int hiScore = 0;
	int score = 0;

	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}


	// For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);

	// Load the font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \n to continue");

	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");

	// LEVELING up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
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

	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");

	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");

	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);
	
	bool UpdateFrame = true;

	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	// Prepare the splat sound
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	sf::Sound splat;
	splat.setBuffer(splatBuffer);

	// Prepare the shoot sound 
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot; 
	shoot.setBuffer(shootBuffer);

	// Prepare the reload sound
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	// Prepare the failed sound
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);

	// Prepare the powerup sound
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	// Prepare the pickup sound
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);


	while (window.isOpen())
	{
		while (window.pollEvent(ev)) {
			if (ev.key.code == Keyboard::Enter && state == State::PLAYING) { state = State::PAUSED; }
			else if (ev.key.code == Keyboard::Enter && state == State::PAUSED)
			{ 
				state == State::PLAYING; 
				clock.restart();
			}
			else if (ev.key.code == Keyboard::Enter && state == State::GAME_OVER) { state = State::LEVELING_UP; }
			else if (state == State::PLAYING) {
				if (ev.key.code == Keyboard::R)
				{
					if (bulletsSpare >= clipSize)
					{
						bulletsSpare -= clipSize - bulletsInClip;
						bulletsInClip = clipSize;
						reload.play();
					}
					else if (bulletsSpare > 0)
					{
						bulletsInClip = bulletsSpare;
						bulletsSpare = 0;
					}
					else
					{
						//TODO
						reloadFailed.play();
					}
					UpdateFrame = true;
				}
			}
		}
		if (Event::Closed == ev.type || Keyboard::isKeyPressed(Keyboard::Escape)) { window.close(); }
		if (state == State::PLAYING) {
			if(Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if( gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
				{
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
					currentBullet++;
					if (currentBullet > 99) {
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					bulletsInClip--;
					shoot.play();
					UpdateFrame = true;
				}
			}
		}
		/*
		*************************************************
		-------------------- UPDATE ---------------------
		*************************************************
		*/
		if(state == State::LEVELING_UP)
		{
			if (ev.key.code == Keyboard::Num1) { 
				fireRate++;
				state = State::PLAYING; 
			}
			if (ev.key.code == Keyboard::Num2) { 
				clipSize += clipSize;
				state = State::PLAYING; 
			}
			if (ev.key.code == Keyboard::Num3) { 
				player.upgradeHealth();
				state = State::PLAYING; 
			}
			if (ev.key.code == Keyboard::Num4) {
				player.upgradeSpeed();
				state = State::PLAYING; 
			}
			if (ev.key.code == Keyboard::Num5) {
				healtPickup.upgrade();
				state = State::PLAYING; 
			}
			if (ev.key.code == Keyboard::Num6) {
				ammoPickup.upgrade();
				state = State::PLAYING; 
			}
			if (state == State::PLAYING) {
				wave++;
				arena.width = 600*wave;
				arena.height = 600*wave;
				arena.left = 0;
				arena.top = 0;
				healtPickup.setArena(arena);
				ammoPickup.setArena(arena);
				int tileSize = createBackgorund(background, arena);
				player.spawn(arena, resolution, tileSize);
				numZombies = 10;
				numZombiesAlive = numZombies;
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				powerup.play();
				clock.restart();
			}
		}
		if (state == State::PLAYING)
		{
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();
			mouseScreenPosition = Vector2f(Mouse::getPosition());
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);
			player.update(dtAsSeconds, Mouse::getPosition());
			Vector2f playerPosition(player.getCenter());
			mainView.setCenter(player.getCenter());
			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive()) {
					zombies[i].Update(dt.asSeconds(), playerPosition);
				}
			}
			for (int i = 0; i < 100; i++) {
				if (bullets[i].isInFlight()) {
					bullets[i].update(dtAsSeconds);
				}
			}
			healtPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);
			spriteCrosshair.setPosition(mouseWorldPosition);
			for (int currentZombieIndex = 0; currentZombieIndex < numZombies; currentZombieIndex++) {
				if (zombies[currentZombieIndex].isAlive()) {
					for (int currentBulletIndex = 0; currentBulletIndex < 100; currentBulletIndex++) {
						if (bullets[currentBulletIndex].isInFlight() && bullets[currentBulletIndex].getPosition().intersects(zombies[currentZombieIndex].getPosition())) {
							bullets[currentBulletIndex].stop();
							if (zombies[currentZombieIndex].hit()) {
								score += 10;
								splat.play();
								numZombiesAlive--;
								if (score > hiScore) {
									hiScore = score;
									std::ofstream outputFile("gamedata/scores.txt");
									outputFile << hiScore;
									outputFile.close();
								}
								if (numZombiesAlive == 0) {
									state = State::LEVELING_UP;
								}
								UpdateFrame = true;
								continue;
							}
						}
					}
					Vector2f collision(player.m_Boundries.getPosition().x - zombies[currentZombieIndex].m_Boundries.getPosition().x, player.m_Boundries.getPosition().y - zombies[currentZombieIndex].m_Boundries.getPosition().y);
					if (sqrt(collision.x*collision.x+collision.y*collision.y) < player.m_Boundries.getRadius() + zombies[currentZombieIndex].m_Boundries.getRadius()) {
						std::cout << currentZombieIndex << std::endl;
						if (player.hit(gameTimeTotal)) {
							hit.play();
							UpdateFrame = true;
						}
						if (player.getHelth() <= 0) {
							state = State::GAME_OVER;
							break;
						}
					}
				}
			}
			if (player.getPosition().intersects(healtPickup.getPosition()) && healtPickup.isSpawned()) {
				player.increaseHealthLevel(healtPickup.gotIt());
				pickup.play();
				UpdateFrame = true;
			}
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned()) {
				bulletsSpare += ammoPickup.gotIt();
				reload.play();
				UpdateFrame = true;
			}
			healthBar.setSize(Vector2f(player.getHelth() * 5, 50));
			if (UpdateFrame) {
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssNumZombiesAlive;
				std::stringstream ssWave;

				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ssScore << "Score: " << score;
				ssHiScore << "Hi score: " << hiScore;
				ssNumZombiesAlive << "Zombies alive: " << numZombiesAlive;
				ssWave << "Wave: " << wave;

				ammoText.setString(ssAmmo.str());
				scoreText.setString(ssScore.str());
				hiScoreText.setString(ssHiScore.str());
				zombiesRemainingText.setString(ssNumZombiesAlive.str());
				waveNumberText.setString(ssWave.str());
				UpdateFrame = false;
			}
		}
		/*
		*************************************************
		---------------------- DRAW ---------------------
		*************************************************
		*/
		if(state == State::PLAYING)
		{
			window.clear();
			window.setView(mainView);
			window.draw(background, &textureBackground);
			for (int i = 0; i < numZombies; i++) {
				window.draw(zombies[i].getSprite());
				window.draw(zombies[i].m_Boundries);
			}
			for (int i = 0; i < 100; i++) {
				if (bullets[i].isInFlight()) {
					window.draw(bullets[i].getShape());
				}
			}
			if (ammoPickup.isSpawned()) {
				window.draw(ammoPickup.getSprite());
			}
			if (healtPickup.isSpawned()) {
				window.draw(healtPickup.getSprite());
			}
			window.draw(spriteCrosshair);
			window.draw(player.getSprite());
			window.draw(player.m_Boundries);
			
			window.setView(hudView);

			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(hiScoreText);
			window.draw(scoreText);
			window.draw(zombiesRemainingText);
			window.draw(ammoText);
		}
		if (state == State::LEVELING_UP) {
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		if (state == State::PAUSED) {
			window.draw(pausedText);
		}
		if (state == State::GAME_OVER) {
			wave = 0;
			score = 0;
			currentBullet = 0;
			bulletsSpare = 24;
			bulletsInClip = 6;
			clipSize = 6;
			fireRate = 1;
			player.restPlayerStats();
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(hiScoreText);
			window.draw(scoreText);
		}
		window.display();
	}
	delete[] zombies;
	return 0;
}