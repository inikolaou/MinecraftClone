#include "Voxel.h"

Voxel::Voxel(TextureManager* textureManager, string name,
	unordered_map<string, string> voxelFaceTextures, unique_ptr<Model> model)
	: m_name{ name } 
{
    m_vertex_positions = model->vertex_positions();
    m_tex_coords = model->tex_coords();
    m_normal_positions = model->normal_positions();
    m_shading_values = model->shading_values();
    transparent = model->is_transparent();
    is_cube = model->is_cube();

    if (name != "air") {
        for (unordered_map<string, string>::iterator it = voxelFaceTextures.begin(); it != voxelFaceTextures.end(); ++it) {
            string texture = it->second;
            textureManager->addTexture(texture);

            int texture_index = find(textureManager->textures.begin(), textureManager->textures.end(), texture) - textureManager->textures.begin();

            if (it->first == "all") {
                set_voxel_face(0, texture_index);
                set_voxel_face(1, texture_index);
                set_voxel_face(2, texture_index);
                set_voxel_face(3, texture_index);
                set_voxel_face(4, texture_index);
                set_voxel_face(5, texture_index);
            }
            else if (it->first == "sides") {
                set_voxel_face(0, texture_index);
                set_voxel_face(1, texture_index);
                set_voxel_face(4, texture_index);
                set_voxel_face(5, texture_index);
            }
            else {
                vector<string> faces = { "right", "left", "top", "bottom", "front", "back" };
                int face_index = find(faces.begin(), faces.end(), it->first) - faces.begin();
                set_voxel_face(face_index, texture_index);
            }
        }
    }
    
}