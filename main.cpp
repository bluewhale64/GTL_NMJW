#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC
#include <stb/stb_image.h>
#include <GL/glew.h>
#include <cstdio>
#include <cstdlib>
#include <stdarg.h>
#include <inttypes.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_joystick.h>

#include "renderer.hpp"
#include "model.hpp"
#include "loader.hpp"
#include "controls.hpp"

int main(void) {

    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    Renderer::init("GTL NMJW", 1, 0.0f, 0.3f, 0.2f, 0.0f);
    if (Renderer::getWindow() == nullptr) {
        std::printf("Window Startup Error\n");
        return -1;
    }

    SDL_Gamepad* controller = nullptr;
    //move controller into render class?
    //make a controller class?
    char* mappingstring = nullptr;
    controller = Controls::findExistingGamepad();
    if(controller){
        mappingstring = SDL_GetGamepadMapping(controller);
        std::printf("Mapping:\n%s\n", mappingstring);
    }
    Loader::init();

    glm::mat4 P = glm::perspective(glm::radians(45.0f), Renderer::getAspect(), 0.1f, 100.0f);
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
    if(Loader::load("assets/TARDIS_blank.bin") != 0){
        std::printf("Model load failed\n");
        return -1;
    }
    SingleColourModel TARDIS(Loader::getVertexBuffer(), Loader::getFloatCount(), Loader::getIndexBuffer(), Loader::getIndexCount(), &SingleColour, &mvp, 0.0, 0.0, 1.0, 1.0);
    glDisable(GL_CULL_FACE);
    
    TARDIS.translate(0, -(TARDIS.getSize().y * 0.75), 0);
    GLfloat framenumber = -1.0;
    
    
    SDL_Event event;
    bool quit = false;
    while (quit == false) {
        framenumber++; 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Test.draw();
        TARDIS.draw();
        TARDIS.translate(0, 0.025 * sin(framenumber/50.0), 0);
        SDL_GL_SwapWindow(Renderer::getWindow());

        //Poll Events
        while(SDL_PollEvent(&event)){
            switch (event.type){
            //switch-case functions like goto, not if-else.
            //every case must end with a break statement.
            case SDL_EVENT_QUIT:
                quit = true;
                break;
            case SDL_EVENT_GAMEPAD_ADDED:
                std::printf("Controller connected.\n");
                if (controller == nullptr) {
                    controller = SDL_OpenGamepad(event.gdevice.which);
                    std::printf("Controller activated.\n");
                    mappingstring = SDL_GetGamepadMapping(controller);
                    std::printf("%s\n", mappingstring);
                }
                break;
            case SDL_EVENT_GAMEPAD_AXIS_MOTION:
                if(event.gaxis.axis != SDL_GAMEPAD_AXIS_INVALID){
                    Controls::setAxis(event.gaxis.axis, event.gaxis.value);
                }
                break;
            case SDL_EVENT_GAMEPAD_BUTTON_UP:
                if(event.gbutton.button != SDL_GAMEPAD_BUTTON_INVALID){
                    Controls::releaseButton(event.gbutton.button);
                }
                break;
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                if(event.gbutton.button != SDL_GAMEPAD_BUTTON_INVALID){
                    Controls::pressButton(event.gbutton.button);
                }
                break;
            //case SDL_EVENT_GAMEPAD_REMOVED:
                //break;
            //wait until stable release
            //necessary function implementations do not exist
            }
        }
        //End of Event Polling
        
        if(Controls::getButton(SDL_GAMEPAD_BUTTON_WEST)){
            TARDIS.translate(0.01, 0, 0);
        }
        if(Controls::getButton(SDL_GAMEPAD_BUTTON_EAST)){
            TARDIS.translate(-0.01, 0, 0);
        }
        if(Controls::getButton(SDL_GAMEPAD_BUTTON_NORTH)){
            TARDIS.translate(0, 0, 0.01);
        }
        if(Controls::getButton(SDL_GAMEPAD_BUTTON_SOUTH)){
            TARDIS.translate(0, 0, -0.01);
        }
        //update control flags right at the end of the main loop
        Controls::updateFlags();
    }
    SDL_CloseGamepad(controller);
    Renderer::stop();
    Loader::stop();
    std::printf("GTL_NMJW closed.\n");
    return 0;
}