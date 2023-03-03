#include "Shader.h"

Shader::Shader(const char* vert_path, const char* frag_path) {
    // Create the shaders
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    compileShader(vertexShaderID, vert_path);

    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    compileShader(fragmentShaderID, frag_path);

    // Link the program
    cout << "Linking shaders... " << endl;
    programID = glCreateProgram();

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    GLint result = GL_FALSE;
    int infoLogLength;
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
        //throw runtime_error(string(&programErrorMessage[0]));
        cout << &programErrorMessage[0] << endl;
    }

    glDetachShader(programID, vertexShaderID);
    glDeleteShader(vertexShaderID);

    glDetachShader(programID, fragmentShaderID);
    glDeleteShader(fragmentShaderID);

    cout << "Shader program complete." << endl;
}

Shader::~Shader() {
	glDeleteProgram(programID);
}

void Shader::use() {
	glUseProgram(programID);
}

GLuint Shader::find_uniform(const char* name) {
    return glGetUniformLocation(programID, name);
}

void Shader::uniform_matrix(GLuint location, mat4 matrix) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}