#pragma once
#include <cstdio>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

/// @brief #include "renderer.hpp"
///
/// Holds various functions and variables for information about the output rendering engine.
namespace Renderer {
    /// @brief 
    /// @param title A string containing the name of the window.
    /// @param swapinterval The swap interval for the window. Determines how often the frame changes. Can be -1, 0 or 1.
    ///
    /// -1 - Adaptive vsync.
    ///
    /// 0 - Update the display after every draw call.
    ///
    /// 1 - Update the display once per frame.
    ///
    /// @param red The red colour value for the window background colour. Can be between 0.0 and 1.0 inclusive.
    /// @param green The green colour value for the window background colour. Can be between 0.0 and 1.0 inclusive.
    /// @param blue The blue colour value for the window background colour. Can be between 0.0 and 1.0 inclusive.
    /// @param alpha The opacity value for the window background colour. Can be between 0.0 and 1.0 inclusive.
    void init(const char* title, int swapinterval, float red, float green, float blue, float alpha);
    /// @brief Switches between fullscreen and windowed mode for the display window.
    void toggleFullscreen();
    /// @brief Gets the width of the display window.
    /// @return The width in pixels of the diplay window.
    int getWidth();
    /// @brief Gets the height of the display window.
    /// @return The height in pixels of the diplay window.
    int getHeight();
    /// @brief Gets the aspect ratio (width divided by height) of the display window.
    /// @return The width in pixels of the diplay window.
    float getAspect();
    /// @brief Get the pointer to the display window.
    /// @return A pointer to the window.
    SDL_Window* getWindow();
    /// @brief Closes the window and terminates SDL.
    ///
    /// Call it at the end of the program.
    void stop();
};