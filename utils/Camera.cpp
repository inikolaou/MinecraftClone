#include "camera.h"

Camera::Camera(GLFWwindow* window, Shader* shader, World* world)
    : window{window}, shader{shader}, world{world}
{
    camera_position = vec3(9, 48, 0);
    horizontalAngle = 0.785f;
    verticalAngle = 0.0f;
    FoV = 45.0f;
    speed = 10.0f;
    mouseSpeed = 0.001f;
    fovSpeed = 2.0f;
    holding = 7;
    matrixLocation = shader->find_uniform("M");
}

void Camera::update() {
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glfwSetCursorPos(window, (float)width / 2, (float)height / 2);

    horizontalAngle += mouseSpeed * ((float)width / 2 - xPos);
    verticalAngle += mouseSpeed * ((float)height / 2 - yPos);

    vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    vec3 right(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );

    vec3 up = cross(right, direction);

    

    

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera_position += direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_position -= direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_position += right * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_position -= right * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera_position += up * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera_position -= up * deltaTime * speed;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (!right_mouse_down) {
            right_mouse_down = true;
            Ray* hit_ray = new Ray(world, horizontalAngle, verticalAngle, camera_position);
            while (hit_ray->m_distance < Ray::range) {
                if (hit_ray->step(2, &holding)) break;
            }
        }
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        right_mouse_down = false;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!left_mouse_down) {
            left_mouse_down = true;
            Ray* hit_ray = new Ray(world, horizontalAngle, verticalAngle, camera_position);
            while (hit_ray->m_distance < Ray::range) {
                if (hit_ray->step(0, &holding)) break;
            }
        }   
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        left_mouse_down = false;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        if (!middle_mouse_down) {
            middle_mouse_down = true;
            Ray* hit_ray = new Ray(world, horizontalAngle, verticalAngle, camera_position);
            while (hit_ray->m_distance < Ray::range) {
                if (hit_ray->step(1, &holding)) break;
            }
        }
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE) {
        middle_mouse_down = false;
    }

    projectionMatrix = perspective(radians(FoV), (float)width / (float)height, 0.1f, 300.0f);
    viewMatrix = lookAt(
        camera_position,
        camera_position + direction,
        up
    );

    lastTime = currentTime;
}