#include "Window.h"
#include "iostream"

Window::Window(int width, int height, const char* title) 
      : width{width}, height{height}, title{title}
{
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);  // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw runtime_error(string(string("Failed to open GLFW window.") +
            " If you have an Intel GPU, they are not 3.3 compatible." +
            "Try the 2.1 version.\n"));
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLEW\n");
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, width / 2, height / 2);

    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);

    model = new Drawable("../src/earth.obj");

    for (int i = 0; i < model->normals.size(); i++) {
        model->normals[i] *= -1.0f;
    }

    model = new Drawable(model->vertices, model->uvs, model->normals);

    world = new World();

    shader = new Shader("../src/basic.vertexshader", "../src/basic.fragmentshader");
    depthProgram = new Shader("../src/depth.vertexshader", "../src/depth.fragmentshader");

    

    shadowViewProjectionLocation = glGetUniformLocation(depthProgram->programID, "VP");
    shadowModelLocation = glGetUniformLocation(depthProgram->programID, "M");
    shaderSamplerLocation = shader->find_uniform("textureArraySampler");
    projectionMatrixLocation = glGetUniformLocation(shader->programID, "P");
    viewMatrixLocation = glGetUniformLocation(shader->programID, "V");
    modelMatrixLocation = glGetUniformLocation(shader->programID, "M");
    LaLocation = glGetUniformLocation(shader->programID, "light.La");
    LdLocation = glGetUniformLocation(shader->programID, "light.Ld");
    LsLocation = glGetUniformLocation(shader->programID, "light.Ls");
    lightPositionLocation = glGetUniformLocation(shader->programID, "light.lightPosition_worldspace");
    lightPowerLocation = glGetUniformLocation(shader->programID, "light.power");
    depthMapSampler = glGetUniformLocation(shader->programID, "shadowMapSampler");
    lightVPLocation = glGetUniformLocation(shader->programID, "lightVP");
    renderLightLocation = glGetUniformLocation(shader->programID, "renderLight");

    initializeDepthBuffer(&depthFrameBuffer, &depthTexture);

    camera = new Camera(window, shader, world);

    light = new Light(window,
        vec4{ 1, 1, 1, 1 },
        vec4{ 1, 1, 1, 1 },
        vec4{ 1, 1, 1, 1 },
        vec3{ 26, 52, 19 },
        100.0f
    );

    
}

void Window::initializeDepthBuffer(GLuint* depthFrameBuffer, GLuint* depthTexture) {
    // Tell opengl to generate a framebuffer
    glGenFramebuffers(1, depthFrameBuffer);
    // Binding the framebuffer, all changes bellow will affect the binded framebuffer
    // **Don't forget to bind the default framebuffer at the end of initialization
    glBindFramebuffer(GL_FRAMEBUFFER, *depthFrameBuffer);


    // We need a texture to store the depth image
    glGenTextures(1, depthTexture);
    glBindTexture(GL_TEXTURE_2D, *depthTexture);
    // Telling opengl the required information about the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0,
        GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // Set color to set out of border 
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // Next go to fragment shader and add an iff statement, so if the distance in the z-buffer is equal to 1, 
    // meaning that the fragment is out of the texture border (or further than the far clip plane) 
    // then the shadow value is 0.
    //*/

    //*/ Task 3.2 Continue
    // Attaching the texture to the framebuffer, so that it will monitor the depth component
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *depthTexture, 0);


    // Since the depth buffer is only for the generation of the depth texture, 
    // there is no need to have a color output
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);


    // Finally, we have to always check that our frame buffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glfwTerminate();
        throw runtime_error("Frame buffer not initialized correctly");
    }

    // Binding the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::depth_pass(mat4 viewMatrix, mat4 projectionMatrix, GLuint depthFBuffer) {
    // Setting viewport to shadow map size
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    // Binding the depth framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthFBuffer);

    // Cleaning the framebuffer depth information (stored from the last render)
    glClear(GL_DEPTH_BUFFER_BIT);

    // Selecting the new shader program that will output the depth component
    depthProgram->use();

    // sending the view and projection matrix to the shader
    mat4 view_projection = projectionMatrix * viewMatrix;
    glUniformMatrix4fv(shadowViewProjectionLocation, 1, GL_FALSE, &view_projection[0][0]);

    mat4 worldModelMatrix = mat4(1.0);
    glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &worldModelMatrix[0][0]);

    world->draw();

    // binding the default framebuffer again
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::uploadLight(const Light& light, GLuint laLoc, GLuint ldLoc, GLuint lsLoc, GLuint lightPosLoc, GLuint lightPowerLoc) {
    glUniform4f(laLoc, light.La.r, light.La.g, light.La.b, light.La.a);
    glUniform4f(ldLoc, light.Ld.r, light.Ld.g, light.Ld.b, light.Ld.a);
    glUniform4f(lsLoc, light.Ls.r, light.Ls.g, light.Ls.b, light.Ls.a);
    glUniform3f(lightPosLoc, light.lightPosition_worldspace.x,
        light.lightPosition_worldspace.y, light.lightPosition_worldspace.z);
    glUniform1f(lightPowerLoc, light.power);
}

void Window::lighting_pass(mat4 viewMatrix, mat4 projectionMatrix) {
    // Step 1: Binding a frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);

    // Step 2: Clearing color and depth info
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Step 3: Selecting shader program
    glUseProgram(shader->programID);

    // Making view and projection matrices uniform to the shader program
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // uploading the light parameters to the shader program
    uploadLight(*light, LaLocation, LdLocation, LsLocation, lightPositionLocation, lightPowerLocation);

    mat4 lightModelMatrix = translate(mat4(), light->lightPosition_worldspace);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &lightModelMatrix[0][0]);

    glUniform1i(renderLightLocation, 1);

    model->bind();
    model->draw();

    glUniform1i(renderLightLocation, 0);

    // Sending the shadow texture to the shaderProgram
    glActiveTexture(GL_TEXTURE0);								// Activate texture position
    glBindTexture(GL_TEXTURE_2D, depthTexture);			// Assign texture to position 
    glUniform1i(depthMapSampler, 0);

    // Sending the light View-Projection matrix to the shader program
    mat4 light_VP = light->lightVP();
    glUniformMatrix4fv(lightVPLocation, 1, GL_FALSE, &light_VP[0][0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY, world->textureManager->textureArray);
    glUniform1i(shaderSamplerLocation, 1);

    mat4 worldModelMatrix = mat4(1.0);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &worldModelMatrix[0][0]);

    world->draw();
}

void Window::on_draw() {
    depth_pass(light->viewMatrix, light->projectionMatrix, depthFrameBuffer);
    camera->update();
    light->update();
    lighting_pass(camera->viewMatrix, camera->projectionMatrix);
}

void Window::run() {
    camera->update();
    light->update();
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window)) {
        on_draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

Window::~Window() {
    glfwTerminate();
}