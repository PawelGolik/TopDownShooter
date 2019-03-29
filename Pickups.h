#pragma once
#include <SFML/Graphics.hpp>

enum PICKUPTYPE {
	Health, Ammo
};

using namespace sf;
class Pickups
{
private:
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECOND_TO_LIVE = 5;
	Sprite m_Sprite;
	IntRect m_Arena;
	int m_Value;
	PICKUPTYPE m_Type;
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsInceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWiat;
public:
	Pickups(PICKUPTYPE Type);
	void setArena(IntRect arena);
	void spawn();
	FloatRect getPosition();
	Sprite getSprite();
	void update(float elapsedTime);
	bool isSpawned();
	int gotIt();
	void upgrade();
	~Pickups();
};

