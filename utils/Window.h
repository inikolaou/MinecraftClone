#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shader.h"
#include "TextureManager.h"
#include "Camera.h"
#include "World.h"
#include "Light.h"
#include "models/Model.h"
#include "Drawable.h"

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 768

using namespace std;
using namespace glm;

class Window
{
public:
	Window(int width, int height, const char* title);
	~Window();

	void on_draw();

	void run();	

private:
	int width;
	int height;
	const char* title;
	GLFWwindow* window;
	GLuint shaderSamplerLocation;
	Shader* shader;
	Shader* depthProgram;
	Camera* camera;
	World* world;
	Light* light;
	Drawable* model;
	GLuint depthFrameBuffer, depthTexture;
	GLuint shadowViewProjectionLocation, shadowModelLocation;
	GLuint depthMapSampler, lightVPLocation;
	GLuint LaLocation, LdLocation, LsLocation;
	GLuint lightPositionLocation, lightPowerLocation;
	GLuint viewMatrixLocation;
	GLuint projectionMatrixLocation;
	GLuint modelMatrixLocation;
	GLuint renderLightLocation;

	void initializeDepthBuffer(GLuint* depthFrameBuffer, GLuint* depthTexture);
	void depth_pass(mat4 viewMatrix, mat4 projectionMatrix, GLuint depthFBuffer);
	void lighting_pass(mat4 viewMatrix, mat4 projectionMatrix);
	void uploadLight(const Light& light, GLuint laLoc, GLuint ldLoc, GLuint lsLoc, GLuint lightPosLoc, GLuint lightPowerLoc);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}
};

#endif /* WINDOW_H */