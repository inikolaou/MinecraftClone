#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include "World.h"

using namespace glm;
using namespace std;

class Ray
{
	
public:
	static constexpr float range = 3.0f;
	Ray(World* world, float horizontal_angle, float vertical_angle, vec3 starting_position);
	bool check(int input, int* holding, float distance, vec3 current_block, vec3 next_block);
	bool step(int input, int* holding);

	float m_distance;
private:
	vec3 u_vector;
	vec3 ray_position;
	vec3 block;
	World* world;
};

#endif /* RAY_H */