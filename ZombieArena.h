#pragma once
#include<SFML/Graphics.hpp>
#include "Zombie.h"


using namespace sf;

int createBackgorund(VertexArray& rVA, IntRect arena);
Zombie* createHorde(int numZombies, IntRect arena);