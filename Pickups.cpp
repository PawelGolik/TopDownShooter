#include "Pickups.h"
#include "TextureHolder.h"


Pickups::Pickups(PICKUPTYPE type)
{
	m_Type = type;
	switch (m_Type)
	{
	case PICKUPTYPE::Ammo:
		m_Value = AMMO_START_VALUE;
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/ammo_pickup.png"));
		break;
	case PICKUPTYPE::Health:
		m_Value = HEALTH_START_VALUE;
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/health_pickup.png"));
		break;
	default:
		break;
	}
	m_Sprite.setOrigin(25, 25);
	m_SecondsToLive = START_SECOND_TO_LIVE;
	m_SecondsToWiat = START_WAIT_TIME;
}

void Pickups::setArena(IntRect arena) {
	m_Arena.left = arena.left + 50;
	m_Arena.width = arena.width - 50;
	m_Arena.top = arena.top + 50;
	m_Arena.height = arena.height - 50;
	spawn();
}

void Pickups::spawn() {
	srand((int)time(0) / (m_Type + 1));
	int x = (rand() % m_Arena.width);
	srand((int)time(0) / (m_Type + 1));
	int y = (rand() % m_Arena.height);

	m_SecondsSinceSpawn = 0;
	m_Spawned = true;

	m_Sprite.setPosition(x, y);
}
FloatRect Pickups::getPosition()
{
	return m_Sprite.getGlobalBounds();
}
Sprite Pickups::getSprite() {
	return m_Sprite;
}
bool Pickups::isSpawned() {
	return m_Spawned;
}
int Pickups::gotIt() {
	m_Spawned = false;
	m_SecondsInceDeSpawn = 0;
	return m_Value;
}

void Pickups::update(float elaspedTime) {
	if (m_Spawned) {
		m_SecondsSinceSpawn += elaspedTime;
	}
	else {
		m_SecondsInceDeSpawn += elaspedTime;
	}
	if (m_SecondsSinceSpawn > m_SecondsToLive && m_Spawned) {
		m_Spawned = false;
		m_SecondsInceDeSpawn = 0;
	}
	if (m_SecondsInceDeSpawn > m_SecondsToWiat && !m_Spawned) {
		spawn();
	}
}
void Pickups::upgrade() {
	if (m_Type == PICKUPTYPE::Health) {
		m_Value += (HEALTH_START_VALUE * .5);
	}
	else if (m_Type == PICKUPTYPE::Ammo) {
		m_Value += (AMMO_START_VALUE * .5);
	}
	m_SecondsToLive += (START_SECOND_TO_LIVE / 10);
	m_SecondsToWiat += (START_WAIT_TIME / 10);
}

Pickups::~Pickups()
{
}
