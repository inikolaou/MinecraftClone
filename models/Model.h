#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <GL/glew.h>
#include <glm/common.hpp>

using namespace std;
using namespace glm;

class Model {
public:
    virtual bool is_transparent() const = 0;
    virtual bool is_cube() const = 0;
    virtual vector<vector<vec3>> vertex_positions() const = 0;
    virtual vector<vector<GLfloat>> tex_coords() const = 0;
    virtual vector<vector<vec3>> normal_positions() const = 0;
    virtual vector<vector<GLfloat>> shading_values() const = 0;
};

#endif /* MODEL_H */