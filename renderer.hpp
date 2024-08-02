#pragma once
#include <cstdio>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

namespace Renderer {
    void init(const char* title, int swapinterval, float red, float green, float blue, float alpha);
    void toggleFullscreen();
    int getWidth();
    int getHeight();
    float getAspect();
    SDL_Window* getWindow();
    void stop();
};