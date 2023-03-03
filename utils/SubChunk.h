#ifndef SUBCHUNK_H
#define SUBCHUNK_H

#include "Chunk.h"
#include "Voxel.h"
#include "World.h"

class Chunk;
class World;

class SubChunk
{
public:
	static const int subchunk_width  = 4;
	static const int subchunk_height = 4;
	static const int subchunk_length = 4;
	vector<vec3> m_mesh_vertex_positions;
	vector<GLfloat> m_mesh_tex_coords;
	vector<vec3> m_mesh_normal_positions;
	vector<GLfloat> m_mesh_shading_values;
	vector<GLuint> m_mesh_indices;
	int m_mesh_index_counter = 0;
	SubChunk(Chunk* parent, vec3 subchunk_position);
	void update_mesh();
	void add_face(int face, Voxel* voxelType, vec3 pos);
private:
	Chunk* m_parent;
	World* m_world;
	vec3 m_subchunk_position;
	vec3 m_local_position;
	vec3 m_position;
};

#endif /* SUBCHUNK_ */