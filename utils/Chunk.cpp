#include "Chunk.h"

Chunk::Chunk(World* world, vec3 chunk_position)
    : world{ world }, chunk_position { chunk_position }
{
	this->m_position = chunk_position * vec3(chunk_width, chunk_height, chunk_length);

    for (int x = 0; x < chunk_width; x++) {
        for (int y = 0; y < chunk_height; y++) {
            for (int z = 0; z < chunk_length; z++) {
                voxels[x][y][z] = 0;
            }
        }
    }

    for (int x = 0; x < (int)(chunk_width/SubChunk::subchunk_width); x++) {
        for (int y = 0; y < (int)(chunk_height / SubChunk::subchunk_height); y++) {
            for (int z = 0; z < (int)(chunk_length / SubChunk::subchunk_length); z++) {
                subchunks[make_tuple(x, y, z)] = new SubChunk(this, vec3(x, y, z));
            }
        }
    }

    mesh_vertex_positions.clear();
    mesh_tex_coords.clear();
    mesh_normal_positions.clear();
    mesh_shading_values.clear();
    mesh_index_counter = 0;
    mesh_indices.clear();
	
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertex_position_vbo);

    glGenBuffers(1, &tex_coord_vbo);

    glGenBuffers(1, &normal_position_vbo);

    glGenBuffers(1, &shading_values_vbo);

    glGenBuffers(1, &ibo);
}

void Chunk::update_subchunk_meshes(){
    for (auto it = subchunks.begin(); it != subchunks.end(); it++) {
        SubChunk* subChunk = subchunks[it->first];
        subChunk->update_mesh();
    }
}

void Chunk::update_at_position(vec3 position) {
    tuple<int, int, int> local = make_tuple(
        ((((int)position.x % SubChunk::subchunk_width) + SubChunk::subchunk_width) % SubChunk::subchunk_width),
        ((((int)position.y % SubChunk::subchunk_height) + SubChunk::subchunk_height) % SubChunk::subchunk_height),
        ((((int)position.z % SubChunk::subchunk_length) + SubChunk::subchunk_length) % SubChunk::subchunk_length)
    );

    tuple<int, int, int> chunk_local = world->getLocalPosition(position);

    tuple<int, int, int> subchunk = make_tuple(
        floor(get<0>(chunk_local) / SubChunk::subchunk_width),
        floor(get<1>(chunk_local) / SubChunk::subchunk_height),
        floor(get<2>(chunk_local) / SubChunk::subchunk_length)
    );

    subchunks[subchunk]->update_mesh();

    if (get<0>(local) == SubChunk::subchunk_width - 1) try_update_subchunk_mesh(make_tuple(get<0>(subchunk) + 1, get<1>(subchunk), get<2>(subchunk)));
    if (get<0>(local) == 0) try_update_subchunk_mesh(make_tuple(get<0>(subchunk) - 1, get<1>(subchunk), get<2>(subchunk)));

    if (get<1>(local) == SubChunk::subchunk_height - 1) try_update_subchunk_mesh(make_tuple(get<0>(subchunk), get<1>(subchunk) + 1, get<2>(subchunk)));
    if (get<1>(local) == 0) try_update_subchunk_mesh(make_tuple(get<0>(subchunk), get<1>(subchunk) - 1, get<2>(subchunk)));

    if (get<2>(local) == SubChunk::subchunk_length - 1) try_update_subchunk_mesh(make_tuple(get<0>(subchunk), get<1>(subchunk), get<2>(subchunk) + 1));
    if (get<2>(local) == 0) try_update_subchunk_mesh(make_tuple(get<0>(subchunk), get<1>(subchunk), get<2>(subchunk) - 1));
}

void Chunk::try_update_subchunk_mesh(tuple<int, int, int> subchunk_position) {
    if (subchunks.find(subchunk_position) != subchunks.end()) {
        subchunks[subchunk_position]->update_mesh();
    }
}

void Chunk::updateMesh() {
    mesh_vertex_positions.clear();
    mesh_tex_coords.clear();
    mesh_normal_positions.clear();
    mesh_shading_values.clear();

    mesh_index_counter = 0;
    mesh_indices.clear();

    for (auto it = subchunks.begin(); it != subchunks.end(); it++) {
        SubChunk* subchunk = subchunks[it->first];

        mesh_vertex_positions.insert(mesh_vertex_positions.end(), subchunk->m_mesh_vertex_positions.begin(), subchunk->m_mesh_vertex_positions.end());
        mesh_tex_coords.insert(mesh_tex_coords.end(), subchunk->m_mesh_tex_coords.begin(), subchunk->m_mesh_tex_coords.end());
        mesh_normal_positions.insert(mesh_normal_positions.end(), subchunk->m_mesh_normal_positions.begin(), subchunk->m_mesh_normal_positions.end());
        mesh_shading_values.insert(mesh_shading_values.end(), subchunk->m_mesh_shading_values.begin(), subchunk->m_mesh_shading_values.end());

        vector<GLuint> mesh_indices_temp;

        for (GLuint index : subchunk->m_mesh_indices) {
            mesh_indices_temp.push_back(index + mesh_index_counter);
        }

        mesh_indices.insert(mesh_indices.end(), mesh_indices_temp.begin(), mesh_indices_temp.end());

        mesh_index_counter += subchunk->m_mesh_index_counter;
    }

    mesh_indices_length = mesh_indices.size();

    send_mesh_data_to_gpu();
}


void Chunk::send_mesh_data_to_gpu() {
    if (!mesh_index_counter)
        return;

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_position_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh_vertex_positions.size() * sizeof(vec3), &mesh_vertex_positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, tex_coord_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh_tex_coords.size() * sizeof(GLfloat),
        &mesh_tex_coords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, normal_position_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh_normal_positions.size() * sizeof(vec3),
        &mesh_normal_positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, shading_values_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh_shading_values.size() * sizeof(GLfloat),
        &mesh_shading_values[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_indices_length * sizeof(GLuint), mesh_indices.data(), GL_STATIC_DRAW);
}

void Chunk::draw() {
    if (!mesh_index_counter) return;

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, mesh_indices_length, GL_UNSIGNED_INT, NULL);
}