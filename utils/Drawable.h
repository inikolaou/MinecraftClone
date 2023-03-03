#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <map>
#include "Util.h"
#include "Texture.h"

using namespace glm;
using namespace std;

static std::vector<unsigned int> VEC_UINT_DEFAUTL_VALUE{};
static std::vector<glm::vec3> VEC_VEC3_DEFAUTL_VALUE{};
static std::vector<glm::vec2> VEC_VEC2_DEFAUTL_VALUE{};
static std::map<std::string, GLuint> MAP_STRING_GLUINT_DEFAULT_VALUE{};


class Drawable {
public:
    Drawable(std::string path);

    Drawable(
        const std::vector<glm::vec3>& vertices,
        const std::vector<glm::vec2>& uvs = VEC_VEC2_DEFAUTL_VALUE,
        const std::vector<glm::vec3>& normals = VEC_VEC3_DEFAUTL_VALUE);

    ~Drawable();

    void bind();

    /* Bind VAO before calling draw */
    void draw(int mode = GL_TRIANGLES);

    std::vector<glm::vec3> vertices, normals, indexedVertices, indexedNormals;
    std::vector<glm::vec2> uvs, indexedUVS;
    std::vector<unsigned int> indices;

    GLuint VAO, verticesVBO, uvsVBO, normalsVBO, elementVBO;

private:
    void createContext();

    void loadOBJ(
        const std::string& path,
        std::vector<glm::vec3>& vertices,
        std::vector<glm::vec2>& uvs,
        std::vector<glm::vec3>& normals,
        std::vector<unsigned int>& indices = VEC_UINT_DEFAUTL_VALUE
    );

    void indexVBO(
        const std::vector<glm::vec3>& in_vertices,
        const std::vector<glm::vec2>& in_uvs,
        const std::vector<glm::vec3>& in_normals,
        std::vector<unsigned int>& out_indices,
        std::vector<glm::vec3>& out_vertices,
        std::vector<glm::vec2>& out_uvs,
        std::vector<glm::vec3>& out_normals
    );
};

#endif /* DRAWABLE_H */