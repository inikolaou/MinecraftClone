#ifndef NUMBERS_H
#define NUMBERS_H

#include <vector>
#include <glm/common.hpp>
#include <GL/glew.h>
#include "Model.h"

using namespace std;
using namespace glm;

class Cube : public Model {
public:
	bool is_transparent() const override { return is_transparent_; }
	bool is_cube() const override { return is_cube_; }
	vector<vector<vec3>> vertex_positions() const override { return vertex_positions_; }
	vector<vector<GLfloat>> tex_coords() const override { return tex_coords_; }
	vector<vector<vec3>> normal_positions() const override { return normal_positions_; }
	vector<vector<GLfloat>> shading_values() const override { return shading_values_; }
private:
	bool is_transparent_ = false;
	bool is_cube_ = true;

	vector<vector<vec3>> vertex_positions_ = {
		{vec3(+0.5f, +0.5f, +0.5f), vec3(+0.5f, -0.5f, +0.5f), vec3(+0.5f, -0.5f, -0.5f), vec3(+0.5f, +0.5f, -0.5f)}, // right
		{vec3(-0.5f, +0.5f, -0.5f), vec3(-0.5f, -0.5f, -0.5f), vec3(-0.5f, -0.5f, +0.5f), vec3(-0.5f, +0.5f, +0.5f)}, // left
		{vec3(-0.5f, +0.5f, -0.5f), vec3(-0.5f, +0.5f, +0.5f), vec3(+0.5f, +0.5f, +0.5f), vec3(+0.5f, +0.5f, -0.5f)}, // top
		{vec3(+0.5f, -0.5f, -0.5f), vec3(+0.5f, -0.5f, +0.5f), vec3(-0.5f, -0.5f, +0.5f), vec3(-0.5f, -0.5f, -0.5f)}, // bottom
		{vec3(-0.5f, +0.5f, +0.5f), vec3(-0.5f, -0.5f, +0.5f), vec3(+0.5f, -0.5f, +0.5f), vec3(+0.5f, +0.5f, +0.5f)}, // front
		{vec3(+0.5f, +0.5f, -0.5f), vec3(+0.5f, -0.5f, -0.5f), vec3(-0.5f, -0.5f, -0.5f), vec3(-0.5f, +0.5f, -0.5f)}  // back
	};

	vector<vector<GLfloat>> tex_coords_ = {
		{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f}, // right
		{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f}, // left
		{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f}, // top
		{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f}, // bottom
		{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f}, // front
		{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f}  // back
	};

	vector<vector<vec3>> normal_positions_ = {
		{vec3(+1.0f, +0.0f, +0.0f), vec3(+1.0f, +0.0f, +0.0f), vec3(+1.0f, +0.0f, +0.0f), vec3(+1.0f, +0.0f, +0.0f)}, // right
		{vec3(-1.0f, +0.0f, +0.0f), vec3(-1.0f, +0.0f, +0.0f), vec3(-1.0f, +0.0f, +0.0f), vec3(-1.0f, +0.0f, +0.0f)}, // left
		{vec3(+0.0f, +1.0f, +0.0f), vec3(+0.0f, +1.0f, +0.0f), vec3(+0.0f, +1.0f, +0.0f), vec3(+0.0f, +1.0f, +0.0f)}, // top
		{vec3(+0.0f, -1.0f, +0.0f), vec3(+0.0f, -1.0f, +0.0f), vec3(+0.0f, -1.0f, +0.0f), vec3(+0.0f, -1.0f, +0.0f)}, // bottom
		{vec3(+0.0f, +0.0f, +1.0f), vec3(+0.0f, +0.0f, +1.0f), vec3(+0.0f, +0.0f, +1.0f), vec3(+0.0f, +0.0f, +1.0f)}, // front
		{vec3(+0.0f, +0.0f, -1.0f), vec3(+0.0f, +0.0f, -1.0f), vec3(+0.0f, +0.0f, -1.0f), vec3(+0.0f, +0.0f, -1.0f)}  // back
	};

	vector<vector<GLfloat>> shading_values_ = {
		{0.80, 0.80, 0.80, 0.80}, // right
		{0.80, 0.80, 0.80, 0.80}, // left
		{1.00, 1.00, 1.00, 1.00}, // top
		{0.49, 0.49, 0.49, 0.49}, // bottom
		{0.92, 0.92, 0.92, 0.92}, // front
		{0.92, 0.92, 0.92, 0.92}  // back
	};
};



#endif /* NUMBERS_H */