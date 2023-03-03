#ifndef WORLD_H
#define WORLD_H

#include "Voxel.h"
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include<limits>
#include "TextureManager.h"
#include "Chunk.h"
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include <map>
#include <tuple>
#include "Chunk.h"
#include "models/Plant.h"
#include "models/Cactus.h"
#include <cmath>
#include <random>

using namespace std;
using namespace glm;

class Chunk;

class World
{
public:
	World();
	void draw();
	int getBlockNumber(vec3 position);
	tuple<int, int, int> getChunkPosition(vec3 position);
	tuple<int, int, int> getLocalPosition(vec3 position);
	bool isOpaqueBlock(vec3 position);
	void setBlock(vec3 position, int number);
	void tryUpdateChunkAtPosition(tuple<int, int, int> chunk_position, vec3 position);
	array<int, 256> createChunkHeightMap(vec3 position);
	void createSmoothTerrain(Chunk* chunk, const array<int, 256>& heightMap);
	vector<Voxel*> voxelTypes;
	TextureManager* textureManager;
private:
	map<tuple<int, int, int>, Chunk*> chunks;
};

#endif /* WORLD_H */