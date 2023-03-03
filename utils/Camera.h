#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Ray.h"
#include "World.h"

using namespace glm;

class World;

class Camera {
public:
    Camera(GLFWwindow* window, Shader* shader, World* world);
    // Initial position : on +Z
    vec3 camera_position;
    mat4 viewMatrix;
    mat4 projectionMatrix;
    // Initial horizontal angle : toward -Z
    float horizontalAngle;
    // Initial vertical angle : none
    float verticalAngle;
    World* world;

    void update();
private:
    Shader* shader;
    GLuint matrixLocation;

    GLFWwindow* window;
    // Field of View
    float FoV;
    float speed; // units / second
    float mouseSpeed;
    float fovSpeed;
    int holding;

    bool left_mouse_down = false;
    bool right_mouse_down = false;
    bool middle_mouse_down = false;
};

#endif
