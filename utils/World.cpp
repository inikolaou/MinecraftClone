#include "World.h"

World::World() {
	textureManager = new TextureManager(16, 16, 256);

	voxelTypes.push_back(nullptr);
	voxelTypes.push_back(new Voxel(textureManager, "cobblestone", { {"all", "cobblestone"} }));
	voxelTypes.push_back(new Voxel(textureManager, "grass", { {"top", "grass"}, {"bottom", "dirt"}, {"sides", "grass_side"} }));
	voxelTypes.push_back(new Voxel(textureManager, "grass_block", { {"all", "grass"} }));
	voxelTypes.push_back(new Voxel(textureManager, "dirt", { {"all", "dirt"} }));
	voxelTypes.push_back(new Voxel(textureManager, "stone", { {"all", "stone"} }));
	voxelTypes.push_back(new Voxel(textureManager, "sand", { {"all", "sand" } }));
	voxelTypes.push_back(new Voxel(textureManager, "planks", { {"all", "planks"} }));
	voxelTypes.push_back(new Voxel(textureManager, "log", { {"top", "log_top"}, {"bottom" , "log_top"}, {"sides" , "log_side"} }));
	voxelTypes.push_back(new Voxel(textureManager, "leaves", { {"all", "leaves"} }));
	voxelTypes.push_back(new Voxel(textureManager, "daisy", { {"all", "daisy" } }, make_unique<Plant>()));
	voxelTypes.push_back(new Voxel(textureManager, "rose", { {"all", "rose" } }, make_unique<Plant>()));
	voxelTypes.push_back(new Voxel(textureManager, "cactus", { {"top", "cactus_top" }, {"bottom", "cactus_bottom"}, {"sides","cactus_side"} }, make_unique<Cactus>()));
	voxelTypes.push_back(new Voxel(textureManager, "dead_bush", { {"all", "dead_bush"} }, make_unique<Plant>()));

	textureManager->generateMipmaps();

	for (int x = 0; x < 16; x++) {
		for (int y = 0; y < 8; y++) {
			for (int z = 0; z < 16; z++) {
				vec3 chunk_position(x, y, z);
				Chunk* current_chunk = new Chunk(this, chunk_position);
				array<int, 256> arr = createChunkHeightMap(chunk_position);

				createSmoothTerrain(current_chunk, arr);
				tuple<int, int, int> tuple_pos = make_tuple(chunk_position.x, chunk_position.y, chunk_position.z);
				chunks[tuple_pos] = current_chunk;
			}
		}
	}

	for (auto it = chunks.begin(); it != chunks.end(); it++) {
		chunks[it->first]->update_subchunk_meshes();
		chunks[it->first]->updateMesh();
	}
}

tuple<int, int, int> World::getChunkPosition(vec3 position) {
	return make_tuple(
		floor(position.x / Chunk::chunk_width),
		floor(position.y / Chunk::chunk_height),
		floor(position.z / Chunk::chunk_length)
	);
}

array<int, 256> World::createChunkHeightMap(vec3 position) {
	array<int, 256> heightMap;

	for (int x = 0; x < Chunk::chunk_length; x++) {
		for (int z = 0; z < Chunk::chunk_width; z++) {
			int voxelX = x + position.x * Chunk::chunk_width;
			int voxelZ = z + position.z * Chunk::chunk_length;

			float value =         glm::simplex(vec2(voxelX / 128.0f, voxelZ / 128.0f));
			float value2 = 0.50 * glm::simplex(vec2(voxelX / 32.0f, voxelZ / 32.0f));
			float value3 = 0.25 * glm::simplex(vec2(voxelX / 16.0f, voxelZ / 16.0f));

			value = value + value2 + value3;
			value = (value + 3) / 6;

			value *= 32 + 32;

			heightMap[z * Chunk::chunk_length + x] = static_cast<int>(value);
		}
	}

	return heightMap;
}

void World::createSmoothTerrain(Chunk* chunk, const array<int, 256>& heightMap) {
	int cy = chunk->m_position.y;

	// Random seed
	random_device rd;

	// Initialize Mersenne Twister pseudo-random number generator
	mt19937 gen(rd());

	uniform_int_distribution<> dis(-100, 100);

	vector<vec3> positions;
	
	for (int z = 0; z < Chunk::chunk_length; z++) {
		for (int x = 0; x < Chunk::chunk_width; x++) {
			int height = heightMap[z * Chunk::chunk_length + x];
			int number = dis(gen);
			for (int y = 0; y < Chunk::chunk_height; y++) {
				int blockY = cy + y;

				if ((chunk->m_position.x/16) < 8 && (chunk->m_position.z/16) < 16) {
					if (blockY <= height) {
						chunk->voxels[x][y][z] = 2;
					}
					else if ((blockY - height < 4 && blockY - height > 0 && (cy + Chunk::chunk_height - height >= 4)) && (number == 83) && (x != 0 && z != 0 && y != 15 && x != Chunk::chunk_width - 1 && z != Chunk::chunk_length - 1) ) {
						chunk->voxels[x][y][z] = 8;
						if (blockY - height == 3) {
							if (y + 1 == 15)
								positions.push_back(vec3(x, y, z));
							else
								positions.push_back(vec3(x, y, z));
						}
					}
					else if ((blockY - height == 1) && (number == 84)) {
						chunk->voxels[x][y][z] = 10;
					}
					else if ((blockY - height == 1) && (number == 80)) {
						chunk->voxels[x][y][z] = 11;
					}
					else {
						chunk->voxels[x][y][z] = 0;
					}
				}
				else {
					if (blockY <= height) {
						chunk->voxels[x][y][z] = 6;
					}
					else if ((blockY - height <= 3 && blockY - height > 0) && (number > 95)) {
						chunk->voxels[x][y][z] = 12;
					}
					else if ((blockY - height == 1) && (number == 80)) {
						chunk->voxels[x][y][z] = 13;
					}
					else {
						chunk->voxels[x][y][z] = 0;
					}
				}
			}
		}
	}

	for (auto& item : positions) {
		chunk->voxels[item.x][item.y + 1][item.z] = 9;
		chunk->voxels[item.x][item.y + 1][item.z + 1] = 9;
		chunk->voxels[item.x][item.y + 1][item.z - 1] = 9;
		chunk->voxels[item.x + 1][item.y + 1][item.z] = 9;
		chunk->voxels[item.x - 1][item.y + 1][item.z] = 9;
	}
}

tuple<int, int, int> World::getLocalPosition(vec3 position) {
	return (
		make_tuple(
		((((int)position.x % Chunk::chunk_width) + Chunk::chunk_width) % Chunk::chunk_width),
		((((int)position.y % Chunk::chunk_height) + Chunk::chunk_height) % Chunk::chunk_height),
		((((int)position.z % Chunk::chunk_length) + Chunk::chunk_length) % Chunk::chunk_length)
		)
	);
}

int World::getBlockNumber(vec3 position) {
	tuple<int, int, int> chunk_position = getChunkPosition(position);

	if (chunks.find(chunk_position) != chunks.end()) {
		
	}
	else {
		return 0;
	}

	tuple<int, int, int> local_position = getLocalPosition(position);

	int block_number = chunks[chunk_position]->voxels[get<0>(local_position)][get<1>(local_position)][get<2>(local_position)];
	return block_number;

}

bool World::isOpaqueBlock(vec3 position) {
	Voxel* voxelType = voxelTypes[getBlockNumber(position)];

	if (voxelType == nullptr || voxelType->m_name == "air") return false;

	return !voxelType->transparent;
}

void World::setBlock(vec3 position, int number) {
	tuple<int, int, int> chunk_position = getChunkPosition(position);

	if (chunks.find(chunk_position) != chunks.end()) {
		
	}
	else {
		if (number == 0) {
			return;
		}
		chunks[chunk_position] = new Chunk(this, vec3(get<0>(chunk_position), get<1>(chunk_position), get<2>(chunk_position)));
	}

	if (getBlockNumber(position) == number) return;

	tuple<int, int, int> local = getLocalPosition(position);

	chunks[chunk_position]->voxels[get<0>(local)][get<1>(local)][get<2>(local)] = number;
	chunks[chunk_position]->update_at_position(position);
	chunks[chunk_position]->updateMesh();

	tuple<int, int, int> chunk_xyz = chunk_position;

	if (get<0>(local) == Chunk::chunk_width - 1) tryUpdateChunkAtPosition(make_tuple(get<0>(chunk_xyz) + 1, get<1>(chunk_xyz), get<2>(chunk_xyz)), vec3(position.x + 1, position.y, position.z));
	if (get<0>(local) == 0) tryUpdateChunkAtPosition(make_tuple(get<0>(chunk_xyz) - 1, get<1>(chunk_xyz), get<2>(chunk_xyz)), vec3(position.x - 1, position.y, position.z));

	if (get<1>(local) == Chunk::chunk_height - 1) tryUpdateChunkAtPosition(make_tuple(get<0>(chunk_xyz), get<1>(chunk_xyz) + 1, get<2>(chunk_xyz)), vec3(position.x, position.y + 1, position.z));
	if (get<1>(local) == 0) tryUpdateChunkAtPosition(make_tuple(get<0>(chunk_xyz), get<1>(chunk_xyz) - 1, get<2>(chunk_xyz)), vec3(position.x, position.y - 1, position.z));

	if (get<2>(local) == Chunk::chunk_length - 1) tryUpdateChunkAtPosition(make_tuple(get<0>(chunk_xyz), get<1>(chunk_xyz), get<2>(chunk_xyz) + 1), vec3(position.x, position.y, position.z + 1));
	if (get<2>(local) == 0) tryUpdateChunkAtPosition(make_tuple(get<0>(chunk_xyz), get<1>(chunk_xyz), get<2>(chunk_xyz) - 1), vec3(position.x, position.y, position.z - 1));
}

void World::tryUpdateChunkAtPosition(tuple<int, int, int> chunk_position, vec3 position) {
	if (chunks.find(chunk_position) != chunks.end()) {
		chunks[chunk_position]->update_at_position(position);
		chunks[chunk_position]->updateMesh();
	}
}

void World::draw() {
	for (auto it = chunks.begin(); it != chunks.end(); it++) {
		chunks[it->first]->draw();
	}
}