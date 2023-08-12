#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC
#include <stb/stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <inttypes.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
//#include <array>
#include "datatypes.hpp"
#include "model.hpp"
#include "loader.hpp"
#include "controller.hpp"

//To-Do:
//CHECK the proper way to update the contents of a vertex buffer.
//SETSIZE procedure in model classes?

/*
SDL_GameController* findController(void) {
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            return SDL_GameControllerOpen(i);
        }
    }
    return nullptr;
}
*/

int main(void) {

    int width = 800;
    int height = 600;
    float aspect = (float)width / (float)height;

    Renderer render(width, height, "GTL NMJW", nullptr, nullptr, 1, 0.0f, 0.3f, 0.2f, 0.0f);
    if (render.window == nullptr) {
        printf("Window Startup Error\n");
        return -1;
    }

    Loader loader;

    /*
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    
    SDL_GameController* controller = findController();
    
    SDL_Event event;
    switch (event.type) {
        case SDL_CONTROLLERDEVICEADDED:
        if (!controller) {
            controller = SDL_GameControllerOpen(event.cdevice.which);
        }
        break;
        case SDL_CONTROLLERDEVICEREMOVED:
        if (controller && event.cdevice.which == SDL_JoystickInstanceID(
            SDL_GameControllerGetJoystick(controller))) {
            SDL_GameControllerClose(controller);
            controller = findController();
        }
        break;
    }
    */

    glm::mat4 P = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    glm::mat4 V = glm::lookAt(
        glm::vec3(1, 0, -10),  // Camera is at (0,0,0) in World Space
        glm::vec3(1, 0, 10),   // Looks this position - Remeber, +Z is INTO the screen.
        glm::vec3(0, 1, 0));   // Head is up (set to 0,-1,0 to look upside-down)
    glm::mat4 M = glm::mat4(1.0f);
    glm::mat4 mvp = P * V * M; // Remember, matrix multiplication is the other way around
    
    GLuint basicindices[6] = { 0,1,2,2,3,0 };
    GLfloat keycorners[24] = {
        -0.5, -0.5, 5.0, 0.0, 0.0, 0,
         0.5, -0.5, 5.0, 1.0, 0.0, 0,
         0.5,  0.5, 5.0, 1.0, 1.0, 0,
        -0.5,  0.5, 5.0, 0.0, 1.0, 0
    };
    Shader TextureShader("shaders/texture_v.glsl", "shaders/texture_f.glsl");
    Texture BlueSquare("textures/bluesquare.png", GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, nullptr);
    Texture Mii("textures/512x512_mii.png", GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, nullptr);
    Shader SubunitShader("shaders/subunit_v.glsl", "shaders/subunit_f.glsl");
    Texture TileSubunit("textures/subunit.png", GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, nullptr);
    
    GLfloat subcorners[36] = {
        -0.5, -0.5, 5.0,  1.5, -0.5, 0.25, 0.25, 0.75, 0.75,
         0.5, -0.5, 5.0, -0.5, -0.5, 0.25, 0.25, 0.75, 0.75,
         0.5,  0.5, 5.0, -0.5,  1.5, 0.25, 0.25, 0.75, 0.75,
        -0.5,  0.5, 5.0,  1.5,  1.5, 0.25, 0.25, 0.75, 0.75
    };
    SubunitModel Test(subcorners, 36, basicindices, 6, &TileSubunit, &SubunitShader, &mvp);

    Shader SingleColour("shaders/singlecolour_v.glsl", "shaders/singlecolour_f.glsl");
    if(loader.load("assets/TARDIS_blank.bin") != 0){
        printf("Model load failed\n");
        return -1;
    }
    SingleColourModel TARDIS(loader.getvertexbuffer(), loader.getfloatcount(), loader.getindexbuffer(), loader.getindexcount(), &SingleColour, &mvp, 0.0, 0.0, 1.0, 1.0);
    glDisable(GL_CULL_FACE);
    while (glfwGetKey(render.window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(render.window) == 0) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Test.draw();
        TARDIS.draw();
        TARDIS.scaleBase(0.999);
        glfwSwapBuffers(render.window);
        glfwPollEvents();
    }
    printf("GTL_NMJW closed.\n");
    return 0;
}