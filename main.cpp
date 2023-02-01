#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC

//Made scene - consists of 3 textures
//Make sprite - now
//Make walk or basic motion

#include <std_image.h>
#include <glew.h>
#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <array>

#include "buffer.hpp"
#include "datatypes.hpp"
//#include "controls.hpp"

int main(void) {

    int width = 800;
    int height = 600;
    float aspect = (float)width / (float)height;

    Renderer render(width, height, "NMJWGJ", nullptr, nullptr, 1, 0.0f, 0.3f, 0.2f, 0.0f);
    if (render.window == nullptr) {
        printf("Window Startup Error\n");
        return -1;
    }

    //glm::mat4 P = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    glm::mat4 P = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 V = glm::lookAt(
        glm::vec3(10, 2.5, 20), // Camera is at (0,0,0), in World Space
        glm::vec3(10, 2.5, 0), // and looks this position (0, 0, -10)
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 M = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = P * V * M; // Remember, matrix multiplication is the other way around
    /*
    unsigned int testindices[] = { 0,1,2,2,3,0 };
    */
    //floor, wall, sky
    float CNR[] = {
        0.0, 0.0,  0.0, 0.0, 0.0, 0,
        0.0, 0.0, -5.0, 0.0, 1.0, 0,
       20.0, 0.0, -5.0, 4.0, 1.0, 0,
       20.0, 0.0,  0.0, 4.0, 0.0, 0,
        0.0, 0.0,  0.0, 0.0, 0.0, 1,
        0.0, 5.0,  0.0, 0.0, 1.0, 1,
        0.0, 5.0, -5.0, 1.0, 1.0, 1,
        0.0, 0.0, -5.0, 1.0, 0.0, 1,
       20.0, 0.0, -5.0, 5.0, 0.0, 1,
       20.0, 5.0, -5.0, 5.0, 1.0, 1,  
       20.0, 5.0,  0.0, 6.0, 1.0, 1,
       20.0, 0.0,  0.0, 6.0, 0.0, 1,
       -5.0, -5.0, -10.0, 0.0, 0.0, 2,
       -5.0, 10.0, -10.0, 0.0, 1.0, 2,
       25.0, 10.0, -10.0, 1.0, 1.0, 2,
       25.0, -5.0, -10.0, 1.0, 0.0, 2,};
    //Until blending functionality is improved, draw non-transparent objects before all transparent objects
    //Then sort transparent objects from nearest to farthest
    //sky, floor, wall
    unsigned int IND[] = {
    12,14,13,12,15,14,
    0,2,1,0,3,2,
    4,6,5,4,7,6,
    6,7,8,6,8,9,
    8,10,9,8,11,10};

    Shader TXS("shaders/texture.vertex", "shaders/texture.fragment");
    Texture TL0("textures/256x256_mossground.png", GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, nullptr);
    Texture TL1("textures/256x256_wall.png", GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, nullptr);
    Texture TL2("textures/256x256_sky.png", GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, nullptr);
    Texture* TLT[] = {&TL0, &TL1, &TL2};
    Model TLM(CNR, 96, IND, 30, TLT, 3, &TXS, &mvp);
    
    while (glfwGetKey(render.window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(render.window) == 0) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        TLM.draw();
        glfwSwapBuffers(render.window);
        glfwPollEvents();
    }
    return 0;
}