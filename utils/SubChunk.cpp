#include "SubChunk.h"

SubChunk::SubChunk(Chunk* parent, vec3 subchunk_position)
	: m_parent{ parent }, m_subchunk_position{ subchunk_position }
{
	this->m_world = parent->world;

	this->m_local_position = vec3(
		subchunk_position.x * subchunk_width,
		subchunk_position.y * subchunk_height,
		subchunk_position.z * subchunk_length
	);

	this->m_position = parent->m_position + this->m_local_position;

    this->m_mesh_vertex_positions.clear();
    this->m_mesh_tex_coords.clear();
    this->m_mesh_normal_positions.clear();
    this->m_mesh_shading_values.clear();

    this->m_mesh_index_counter = 0;
    this->m_mesh_indices.clear();
}

void SubChunk::update_mesh() {
    this->m_mesh_vertex_positions.clear();
    this->m_mesh_tex_coords.clear();
    this->m_mesh_normal_positions.clear();
    this->m_mesh_shading_values.clear();

    this->m_mesh_index_counter = 0;
    this->m_mesh_indices.clear();

    int local_x, local_y, local_z;
    for (local_x = 0; local_x < subchunk_width; local_x++) {
        for (local_y = 0; local_y < subchunk_height; local_y++) {
            for (local_z = 0; local_z < subchunk_length; local_z++) {
                vec3 parent_local = this->m_local_position + vec3(local_x, local_y, local_z);

                int voxel_number = this->m_parent->voxels[parent_local.x][parent_local.y][parent_local.z];

                if (voxel_number) {
                    Voxel* voxelType = this->m_world->voxelTypes[voxel_number];

                    vec3 pos = this->m_position + vec3(local_x, local_y, local_z);

                    if (voxelType->is_cube)
                    {
                        if (!(this->m_world->isOpaqueBlock(vec3(pos.x + 1, pos.y, pos.z)))) add_face(0, voxelType, pos);
                        if (!(this->m_world->isOpaqueBlock(vec3(pos.x - 1, pos.y, pos.z)))) add_face(1, voxelType, pos);
                        if (!(this->m_world->isOpaqueBlock(vec3(pos.x, pos.y + 1, pos.z)))) add_face(2, voxelType, pos);
                        if (!(this->m_world->isOpaqueBlock(vec3(pos.x, pos.y - 1, pos.z)))) add_face(3, voxelType, pos);
                        if (!(this->m_world->isOpaqueBlock(vec3(pos.x, pos.y, pos.z + 1)))) add_face(4, voxelType, pos);
                        if (!(this->m_world->isOpaqueBlock(vec3(pos.x, pos.y, pos.z - 1)))) add_face(5, voxelType, pos);
                    }
                    else {
                        for (int i = 0; i < voxelType->m_vertex_positions.size(); i++) {
                            add_face(i, voxelType, pos);
                        }
                    }
                }
            }
        }
    }
}

void SubChunk::add_face(int face, Voxel* voxelType, vec3 pos) {
    vector<vec3> vertex_positions = voxelType->m_vertex_positions[face];

    for (int i = 0; i < 4; i++) {
        vertex_positions[i] += pos;
    }

    this->m_mesh_vertex_positions.insert(this->m_mesh_vertex_positions.end(), vertex_positions.begin(), vertex_positions.end());

    vector<GLuint> indices = { 0, 1, 2, 0, 2, 3 };

    for (int i = 0; i < 6; i++) {
        indices[i] += this->m_mesh_index_counter;
    }

    this->m_mesh_indices.insert(this->m_mesh_indices.end(), indices.begin(), indices.end());

    this->m_mesh_index_counter += 4;

    this->m_mesh_tex_coords.insert(this->m_mesh_tex_coords.end(), voxelType->m_tex_coords[face].begin(), voxelType->m_tex_coords[face].end());
    this->m_mesh_normal_positions.insert(this->m_mesh_normal_positions.end(), voxelType->m_normal_positions[face].begin(), voxelType->m_normal_positions[face].end());
    this->m_mesh_shading_values.insert(this->m_mesh_shading_values.end(), voxelType->m_shading_values[face].begin(), voxelType->m_shading_values[face].end());
}