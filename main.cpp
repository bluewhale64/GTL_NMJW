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
        glm::vec3(10, 4, 17.5), // Camera is at (0,0,0), in World Space
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
    //floor, wall
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
       20.0, 0.0,  0.0, 6.0, 0.0, 1
       };
    //Until blending functionality is improved, draw non-transparent objects before all transparent objects
    //Then sort transparent objects from nearest to farthest
    //sky, floor, wall
    unsigned int IND[] = {
    0,2,1,0,3,2,
    4,6,5,4,7,6,
    6,7,8,6,8,9,
    8,10,9,8,11,10
    };

    float psk[] = {
        -5.0,  0.0, -10.0, -0.2, -0.2, 0.0, 0.0, 0, 1,
        -5.0, 15.0, -10.0, -0.2,  0.8, 0.0, 2.3, 0, 1,
        25.0, 15.0, -10.0,  0.8,  0.8, 4.6, 2.3, 0, 1,
        25.0,  0.0, -10.0,  0.8, -0.2, 4.6, 0.0, 0, 1 };

    unsigned int isk[] = {
        0,2,1,0,3,2
    };

    Shader TXS("shaders/texture.vertex", "shaders/texture.fragment");
    Shader DMS("shaders/displacement.vertex", "shaders/displacement.fragment");
    Texture TL0("textures/256x256_mossground.png", GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, nullptr);
    Texture TL1("textures/256x256_wall.png", GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, nullptr);
    Texture SK0("textures/256x256_sky.png", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, nullptr);
    Texture SK1("textures/64x64_skydmp.png", GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, nullptr);
    Texture* TLT[] = {&TL0, &TL1};
    Texture* tsk[] = { &SK0, &SK1 };
    DispModel SKY(psk, 40, isk, 6, tsk, 2, &DMS, &mvp, 0, 0, 0.001, 0.005);
    Model TLM(CNR, 72, IND, 30, TLT, 2, &TXS, &mvp);
    
    while (glfwGetKey(render.window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(render.window) == 0) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SKY.draw();
        TLM.draw();
        glfwSwapBuffers(render.window);
        glfwPollEvents();
    }
    return 0;
}