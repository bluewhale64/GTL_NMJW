#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC
#include <stb/stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include "buffer.hpp"
#include "datatypes.hpp"

//#include "controls.hpp"

int main(void) {

    int width = 800;
    int height = 600;
    float aspect = (float)width / (float)height;

    Renderer render(width, height, "GTL_NMJW", nullptr, nullptr, 1, 0.0f, 0.3f, 0.2f, 0.0f);
    if (render.window == nullptr) {
        printf("Window Startup Error\n");
        return -1;
    }


    glm::mat4 P = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    // Camera (View) matrix
    glm::mat4 V = glm::lookAt(
        glm::vec3(0, 0, 0), // Camera is at (0,0,0), in World Space
        glm::vec3(0, 0, 10), // and looks this position (0, 0, 10) - Remeber, +Z is INTO the screen.
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    glm::mat4 M = glm::mat4(1.0f);
    glm::mat4 mvp = P * V * M; // Remember, matrix multiplication is the other way around
    
    unsigned int basicindices[6] = { 0,1,2,2,3,0 };
    float keycorners[24] = {
        -0.5, -0.5, 5, 0, 0, 0,
         0.5, -0.5, 5, 1, 0, 0,
         0.5,  0.5, 5, 1, 1, 0,
        -0.5,  0.5, 5, 0, 1, 0
    };
    Shader TextureShader("shaders/texture.vertex", "shaders/texture.fragment");
    int result = 0;
    glGetProgramiv(TextureShader.program, GL_VALIDATE_STATUS, &result);
    printf("%i\n", result);
    Texture BlueSquare("textures/bluesquare.png", GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, nullptr);
    Texture* KeyTextures[] = { &BlueSquare };
    Model A_Key(keycorners, 24, basicindices, 6, KeyTextures, 1, &TextureShader, &mvp);

    while (glfwGetKey(render.window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(render.window) == 0) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        A_Key.draw();
        glfwSwapBuffers(render.window);
        glfwPollEvents();
    }
    return 0;
}