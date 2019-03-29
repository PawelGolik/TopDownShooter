#include "ZombieArena.h"

int createBackgorund(VertexArray & rVA, IntRect arena)
{
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;

	int worldWidth = arena.width / TILE_SIZE;
	int worldHeigth = arena.height / TILE_SIZE;
	rVA.setPrimitiveType(Quads);
	rVA.resize(worldHeigth*worldWidth*VERTS_IN_QUAD);
	int currentVertex = 0;
	for (size_t w = 0; w < worldWidth; w++)
	{
		for (size_t h = 0; h < worldHeigth; h++)
		{
			rVA[currentVertex + 0].position = Vector2f(w*TILE_SIZE, h*TILE_SIZE);
			rVA[currentVertex + 1].position = Vector2f((w+1)*TILE_SIZE, h*TILE_SIZE);
			rVA[currentVertex + 2].position = Vector2f((w+1)*TILE_SIZE, (h+1)*TILE_SIZE);
			rVA[currentVertex + 3].position = Vector2f(w*TILE_SIZE, (h+1)*TILE_SIZE);
			if (h == 0 || w == 0 || h == worldHeigth - 1 || w == worldWidth - 1) {
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);
			}
			else
			{
				srand((int)time(0) + h * w - h);
				int mOrG = (rand() % TILE_TYPES);
				int veticalOffset = mOrG * TILE_SIZE;
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + veticalOffset);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + veticalOffset);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + veticalOffset);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + veticalOffset);
			}
			currentVertex = currentVertex + VERTS_IN_QUAD;
		}
	}
	return  TILE_SIZE;
}