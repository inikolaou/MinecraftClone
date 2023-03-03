#include "Ray.h"

Ray::Ray(World* world, float horizontal_angle, float vertical_angle, vec3 starting_position) 
	: world{ world }, ray_position{ starting_position }
{
	u_vector = vec3(
		cos(vertical_angle) * sin(horizontal_angle),
		sin(vertical_angle),
		cos(vertical_angle) * cos(horizontal_angle)
	);

	block = vec3(
		(int) round(ray_position.x),
		(int) round(ray_position.y),
		(int) round(ray_position.z)
	);

	m_distance = 0;
}

bool Ray::check(int input, int* holding, float distance, vec3 current_block, vec3 next_block) {
	if (world->getBlockNumber(next_block))
	{
		switch (input) {
		case 0:
			world->setBlock(next_block, 0);
			break;
		case 1:
			*holding = world->getBlockNumber(next_block);
			break;
		case 2:
			world->setBlock(current_block, *holding);
			break;
		};
		return true;
	}
	else {
		ray_position = ray_position + u_vector * distance;
		block = next_block;
		m_distance += distance;
		return false;
	}
}

bool Ray::step(int input, int* holding) {
	vec3 local_position = ray_position - block;
	vec3 absolute_vector = u_vector;

	vector<int> sign = { 1, 1, 1 };

	if (u_vector.x < 0) {
		sign[0] = -1;
		absolute_vector.x = -absolute_vector.x;
		local_position.x = -local_position.x;
	}
	if (u_vector.y < 0) {
		sign[1] = -1;
		absolute_vector.y = -absolute_vector.y;
		local_position.y = -local_position.y;
	}
	if (u_vector.z < 0) {
		sign[2] = -1;
		absolute_vector.z = -absolute_vector.z;
		local_position.z = -local_position.z;
	}

	if (absolute_vector.x) {
		float x = 0.5f;
		float y = (0.5f - local_position.x) / absolute_vector.x * absolute_vector.y + local_position.y;
		float z = (0.5f - local_position.x) / absolute_vector.x * absolute_vector.z + local_position.z;

		if (y >= -0.5f && y <= 0.5f && z >= -0.5f && z <= 0.5f) {
			float distance = sqrt(pow(x - local_position.x, 2) + pow(y - local_position.y, 2) + pow(z - local_position.z, 2));
			return check(input, holding, distance, block, vec3(block.x + sign[0], block.y, block.z));
		}
	}

	if (absolute_vector.y) {
		float x = (0.5f - local_position.y) / absolute_vector.y * absolute_vector.x + local_position.x;
		float y = 0.5f;
		float z = (0.5f - local_position.y) / absolute_vector.y * absolute_vector.z + local_position.z;

		if (x >= -0.5f && x <= 0.5f && z >= -0.5f && z <= 0.5f) {
			float distance = sqrt(pow(x - local_position.x, 2) + pow(y - local_position.y, 2) + pow(z - local_position.z, 2));
			return check(input, holding, distance, block, vec3(block.x, block.y + sign[1], block.z));
		}
	}

	if (absolute_vector.z) {
		float x = (0.5f - local_position.z) / absolute_vector.z * absolute_vector.x + local_position.x;
		float y = (0.5f - local_position.z) / absolute_vector.z * absolute_vector.y + local_position.y;
		float z = 0.5f; 

		if (x >= -0.5f && x <= 0.5f && y >= -0.5f && y <= 0.5f) {
			float distance = sqrt(pow(x - local_position.x, 2) + pow(y - local_position.y, 2) + pow(z - local_position.z, 2));
			return check(input, holding, distance, block, vec3(block.x, block.y, block.z + sign[2]));
		}
	}
}