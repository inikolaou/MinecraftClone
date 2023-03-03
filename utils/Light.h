#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glfw3.h>
#include <iostream>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

class Light {
public:

    GLFWwindow* window;
    // Light parameters
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    glm::vec3 lightPosition_worldspace;

    glm::vec4 La;
    glm::vec4 Ld;
    glm::vec4 Ls;
    float power;

    float nearPlane;
    float farPlane;

    bool orthoProj;

    float lightSpeed;
    glm::vec3 direction;

    // Where the light will look at
    glm::vec3 targetPosition;

    // Constructor
    Light(GLFWwindow* window,
        glm::vec4 init_La,
        glm::vec4 init_Ld,
        glm::vec4 init_Ls,
        glm::vec3 init_position,
        float init_power);

    void update();

    glm::mat4 lightVP();
};

#endif /* LIGHT_H */