#ifndef PLANT_H
#define PLANT_H

#include <vector>
#include <glm/common.hpp>
#include <GL/glew.h>
#include "Model.h"

using namespace std;
using namespace glm;

class Plant : public Model {
public:
	bool is_transparent() const override { return is_transparent_; }
	bool is_cube() const override { return is_cube_; }
	vector<vector<vec3>> vertex_positions() const override { return vertex_positions_; }
	vector<vector<GLfloat>> tex_coords() const override { return tex_coords_; }
	vector<vector<vec3>> normal_positions() const override { return normal_positions_; }
	vector<vector<GLfloat>> shading_values() const override { return shading_values_; }
private:
	bool is_transparent_ = true;
	bool is_cube_ = false;

	vector<vector<vec3>> vertex_positions_ = {
		{vec3(-0.3536, 0.5000,  0.3536), vec3(-0.3536, -0.5000,  0.3536), vec3( 0.3536, -0.5000, -0.3536), vec3( 0.3536, 0.5000, -0.3536)},
		{vec3(-0.3536, 0.5000, -0.3536), vec3(-0.3536, -0.5000, -0.3536), vec3( 0.3536, -0.5000,  0.3536), vec3( 0.3536, 0.5000,  0.3536)},
		{vec3( 0.3536, 0.5000, -0.3536), vec3( 0.3536, -0.5000, -0.3536), vec3(-0.3536, -0.5000,  0.3536), vec3(-0.3536, 0.5000,  0.3536)},
		{vec3( 0.3536, 0.5000,  0.3536), vec3( 0.3536, -0.5000,  0.3536), vec3(-0.3536, -0.5000, -0.3536), vec3(-0.3536, 0.5000, -0.3536)}
	};

	vector<vector<GLfloat>> tex_coords_ = {
		{0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0}
	};

	vector<vector<vec3>> normal_positions_ = {
		{vec3(+glm::sqrt(2) / 2, +0.0f , +glm::sqrt(2) / 2), vec3(+glm::sqrt(2) / 2, +0.0f , +glm::sqrt(2) / 2), vec3(+glm::sqrt(2) / 2, +0.0f , +glm::sqrt(2) / 2), vec3(+glm::sqrt(2) / 2, +0.0f , +glm::sqrt(2) / 2)},
		{vec3(-glm::sqrt(2) / 2, +0.0f , +glm::sqrt(2) / 2), vec3(-glm::sqrt(2) / 2, +0.0f , +glm::sqrt(2) / 2), vec3(-glm::sqrt(2) / 2, +0.0f , +glm::sqrt(2) / 2), vec3(-glm::sqrt(2) / 2, +0.0f , +glm::sqrt(2) / 2)},
		{vec3(-glm::sqrt(2) / 2, +0.0f , -glm::sqrt(2) / 2), vec3(-glm::sqrt(2) / 2, +0.0f , -glm::sqrt(2) / 2), vec3(-glm::sqrt(2) / 2, +0.0f , -glm::sqrt(2) / 2), vec3(-glm::sqrt(2) / 2, +0.0f , -glm::sqrt(2) / 2)},
		{vec3(+glm::sqrt(2) / 2, +0.0f , -glm::sqrt(2) / 2), vec3(+glm::sqrt(2) / 2, +0.0f , -glm::sqrt(2) / 2), vec3(+glm::sqrt(2) / 2, +0.0f , -glm::sqrt(2) / 2), vec3(+glm::sqrt(2) / 2, +0.0f , -glm::sqrt(2) / 2)},
	};

	vector<vector<GLfloat>> shading_values_ = {
		{1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0}
	};
};

#endif /* PLANT_H */