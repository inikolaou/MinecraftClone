#ifndef VOXEL_H
#define VOXEL_H

#include "models/Model.h"
#include "models/Cube.h"
#include "TextureManager.h"
#include <unordered_map>
#include <memory>

using namespace std;

class Voxel
{
public:
	Voxel(TextureManager* textureManager, string name = "unkown",
		unordered_map<string, string> voxelFaceTextures = { {"all", "cobblestone"} }, unique_ptr<Model> model = make_unique<Cube>());
	Voxel() = default;
	vector<vector<vec3>> m_vertex_positions;
	vector<vector<GLfloat>> m_tex_coords;
	vector<vector<vec3>> m_normal_positions;
	vector<vector<GLfloat>> m_shading_values;
	string m_name;

	bool transparent;
	bool is_cube;
private:
	unique_ptr<Model> model;

	void set_voxel_face(int face, int texture) {
		if (face > m_tex_coords.size() - 1) return;

		for (int vertex = 0; vertex < 4; vertex++) {
			m_tex_coords[face][vertex * 3 + 2] = texture;
		}
	}
};

#endif /* VOXEL_H */