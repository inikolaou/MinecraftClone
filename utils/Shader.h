#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class Shader
{
public:
	Shader(const char* vert_path, const char* frag_path);
	~Shader();

    GLuint programID;
	void use();
    GLuint find_uniform(const char* name);
    void uniform_matrix(GLuint location, mat4 matrix);
private:
	GLuint vertexShaderID;
	GLuint fragmentShaderID;

    static void compileShader(GLuint& shaderID, const char* file_path) {
        // read shader code from the file
        std::string shaderCode;
        std::ifstream shaderStream(file_path, std::ios::in);
        if (shaderStream.is_open()) {
            std::string Line = "";
            while (getline(shaderStream, Line)) {
                shaderCode += "\n" + Line;
            }
            shaderStream.close();
        }
        else {
            throw runtime_error(string("Can't open shader file: ") + file_path);
        }

        GLint result = GL_FALSE;
        int infoLogLength;

        // compile Vertex Shader
        cout << "Compiling shader: " << file_path << endl;
        char const* sourcePointer = shaderCode.c_str();
        glShaderSource(shaderID, 1, &sourcePointer, NULL);
        glCompileShader(shaderID);

        // check Vertex Shader
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            std::vector<char> shaderErrorMessage(infoLogLength + 1);
            glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
            //throw runtime_error(string(&shaderErrorMessage[0]));
            cout << &shaderErrorMessage[0] << endl;
        }
    }
};

#endif /* SHADER_H */