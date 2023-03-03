#ifndef CHUNK_H
#define CHUNK_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <tuple>
#include <map>
#include "World.h"
#include "SubChunk.h"
#include <cmath>

using namespace std;
using namespace glm;

class World;
class SubChunk;


class Chunk
{
public:
	static const int chunk_width = 16;
	static const int chunk_height = 16;
	static const int chunk_length = 16;
	static const int chunk_area = 16*16;
	array<array<array<int, chunk_length>, chunk_height>, chunk_width> voxels;
	World* world;
	vec3 m_position;

	Chunk(World* world, vec3 chunk_position);
	void update_subchunk_meshes();
	void update_at_position(vec3 position);
	void try_update_subchunk_mesh(tuple<int, int, int> subchunk_position);
	void send_mesh_data_to_gpu();
	void updateMesh();
	void draw();
private:
	vec3 chunk_position;
	vector<vec3> mesh_vertex_positions;
	vector<GLfloat> mesh_tex_coords;
	vector<vec3> mesh_normal_positions;
	vector<GLfloat> mesh_shading_values;
	int mesh_index_counter = 0;
	vector<GLuint> mesh_indices;
	int mesh_indices_length = 0;

	map<tuple<int, int, int>, SubChunk*> subchunks;

	GLuint vao;
	GLuint vertex_position_vbo;
	GLuint tex_coord_vbo;
	GLuint normal_position_vbo;
	GLuint shading_values_vbo;
	GLuint ibo;
};

#endif /* CHUNK_H */