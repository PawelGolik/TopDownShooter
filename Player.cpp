#include "Player.h"
#include <iostream>
#include "TextureHolder.h"



Player::Player()
{
	m_Speed = START_SPEED;
	m_Health = START_HELTH;
	m_MaxHealth = START_HELTH;
	m_Texture = TextureHolder::GetTexture("graphics/player.png");
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setOrigin(25, 25);
	m_Boundries.setRadius(25); 
	m_Boundries.setFillColor(Color(0, 0, 0, 0));
	m_Boundries.setOrigin(25, 25);
}
void Player::spawn(IntRect arena, Vector2f resolution, int tileSize) 
{
	m_position.x = m_Arena.width / 2;
	m_position.y = m_Arena.height / 2;
	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;
	m_TileSize = tileSize;
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
}
void Player::restPlayerStats()
{
	m_Speed = START_SPEED;
	m_Health = START_HELTH;
	m_MaxHealth = START_HELTH;
}

Time Player::getLastHitTime()
{
	return m_LastHit;
}

bool Player::hit(Time timeHit) 
{
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200)
	{
		m_LastHit = timeHit;
		m_Health -= 10;
		return true;
	}
	return false;
}

FloatRect Player::getPosition() 
{
	return m_Sprite.getGlobalBounds();
}

Vector2f Player::getCenter() 
{
	return m_position;
}

float Player::getRotation()
{
	return m_Sprite.getRotation();
}

Sprite Player::getSprite()
{
	return m_Sprite;
}

int Player::getHelth() 
{
	return m_Health;
}

void Player::moveRight()
{
	m_RightPressed = true;
}

void Player::moveDown() 
{
	m_DownPressed = true;
}

void Player::moveLeft()
{
	m_LeftPressed = true;
}

void Player::moveUp()
{
	m_UpPressed = true;
}

void Player::stopDown()
{
	m_DownPressed = false;
}

void Player::stopLeft()
{
	m_LeftPressed = false;
}

void Player::stopRight()
{
	m_RightPressed = false;
}

void Player::stopUp()
{
	m_UpPressed = false;
}

void Player::update(float elapsedTime, Vector2i mousePosition)
{
	if (m_UpPressed) { m_position.y -= m_Speed * elapsedTime; }
	if (m_DownPressed) { m_position.y += m_Speed * elapsedTime; }
	if (m_LeftPressed) { m_position.x -= m_Speed * elapsedTime;	}
	if (m_RightPressed) { m_position.x += m_Speed * elapsedTime; }

	m_Sprite.setPosition(m_position);

	if (m_position.x > m_Arena.width - m_TileSize) { m_position.x = m_Arena.width - m_TileSize; }
	if (m_position.x < m_Arena.left + m_TileSize) { m_position.x = m_TileSize; }
	if (m_position.x < m_Arena.left + m_TileSize) { m_position.x = m_TileSize; }
	if (m_position.y > m_Arena.height - m_TileSize) { m_position.y = m_Arena.height - m_TileSize; }
	if (m_position.y < m_Arena.top + m_TileSize) { m_position.y = m_TileSize; }
	
	float angle = float(atan2(mousePosition.y - m_Resolution.y / 2, mousePosition.x - m_Resolution.x / 2) * 180) / 3.141;
	
	m_Sprite.setRotation(angle);
	m_Boundries.setRotation(angle);
	m_Boundries.setPosition(m_position);
}

void Player::upgradeSpeed()
{
	m_Speed += (START_SPEED *.2f);
}

void Player::upgradeHealth()
{
	m_Health += int(START_HELTH *.2f);
}

void Player::increaseHealthLevel(int amount)
{
	m_Health += amount;
	if(m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}

Player::~Player()
{
}
