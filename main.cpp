#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC

//To do: Squash it into 2D mode

//Orthographic matrix

#include <std_image.h>
#include <glew.h>
#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "buffer.hpp"
#include "datatypes.hpp"
#include "controls.hpp"

int main(void) {

    int width = 800;
    int height = 600;
    float aspect = (float)width / (float)height;

    Renderer render(width, height, "NMJWGJ", nullptr, nullptr, 1, 0.0f, 0.3f, 0.2f, 0.0f);
    if (render.window == nullptr) {
        printf("Window Startup Error\n");
        return -1;
    }


    glm::mat4 P = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    //glm::mat4 P = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 V = glm::lookAt(
        glm::vec3(0, 0, -1), // Camera is at (0,0,0), in World Space
        glm::vec3(0, 0, -1), // and looks this position (0, 0, -10)
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 M = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = P * V * M; // Remember, matrix multiplication is the other way around

    float testcorners[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0
    };
    unsigned int testindices[] = { 0,1,2,2,3,0 };

    float border[4] = { 0.31, 0.45, 0.73, 1.0 };
    Texture BWH("textures/512x512_hh.png", GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, border);
    Shader TXS("shaders/2Dtexture.vertex", "shaders/2Dtexture.fragment");
    Texture* testpics[] = { &BWH };
    Model NHM(testcorners, 24, testindices, 6, testpics, 1, &TXS, &mvp);

    Texture BLK("textures/OldFSS02.png", GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, nullptr);
    Texture DPM("textures/OldFSS00.png", GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, nullptr);

    Texture GND("textures/cobble_stone.jpg", GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, nullptr);
    Texture DM0("textures/512x512_mii.png", GL_LINEAR, GL_LINEAR, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT, nullptr);

    Shader DPT("shaders/displacement.vertex", "shaders/displacement.fragment");
    Texture* testex[] = {&GND, &BLK};
    float testcs[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.3f, 0.0f, 1.0f, 0, 1,
         1.0f,  1.0f, 0.0f, 1.3f, 1.3f, 1.0f, 1.0f, 0, 1,
         1.0f, -1.0f, 0.0f, 1.3f, 0.0f, 1.0f, 0.0f, 0, 1
    };
    DispModel Test(testcs, 36, testindices, 6, testex, 2, &DPT, &mvp, 0.0005, 0.0007, 0.0000, 0.000);
    //Texture is currently being used as its own displacement map - BUG
    //either in shader or dismodel.draw

    while (glfwGetKey(render.window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(render.window) == 0) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Test.draw();
        glfwSwapBuffers(render.window);
        glfwPollEvents();
        computeMatricesFromInputs(render.window, width, height, aspect);
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

    }

    return 0;
}