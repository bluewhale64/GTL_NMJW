#include "renderer.hpp"
#include <GL/glew.h>

/// @brief This anonymous (unnamed) namespace holds the rendering engine flags.
///
/// It can't be accessed by functions written outsie of this file (renderer.cpp).
///
/// The functions here can access the data and pass it to code outside this file.
namespace{
    /// @brief The OpenGL rendering context for the window.
    ///
    /// Enables SDL to collect the rendered output from OpenGL and display it in the window.
    SDL_GLContext context;
    /// @brief  The instance ID handle for the display window.
    SDL_DisplayID displayid;
    /// @brief A pointer to an SDL_DisplayMode struct containing the display flags.
    const SDL_DisplayMode* displaymode;
    /// @brief A flag to remember if the window is currently fullscreen or not.
    GLboolean fullscreen = GL_FALSE;
    /// @brief A pointer to the SDL window used by the application.
    SDL_Window* window;
}
void Renderer::init(const char* title, int swapinterval, float red, float green, float blue, float alpha) {
    //Initialize the library
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) != 0) {
        std::printf("SDL initiation failed.\n");
    }
    std::printf("SDL3 initiated.\n");
    fullscreen = GL_FALSE;
    displayid = SDL_GetPrimaryDisplay();
    displaymode = SDL_GetDesktopDisplayMode(displayid);
    window = SDL_CreateWindow(title, displaymode->w, displaymode->h, SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        std::printf("Failed to create SDL3 window.\n");
        SDL_Quit();
    }
    context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(swapinterval);

    if (glewInit() != GLEW_OK) {
        std::printf("Failed to initialise GLEW.\n");
        SDL_Quit();
    }
    std::printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
    glClearColor(red, green, blue, alpha);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}        
void Renderer::toggleFullscreen(){
    if (fullscreen == GL_FALSE){
        SDL_SetWindowFullscreen(window, SDL_TRUE);
        SDL_ShowCursor();
        fullscreen = GL_TRUE;
    }
    else{
        SDL_SetWindowFullscreen(window, SDL_FALSE);
        SDL_ShowCursor();
        fullscreen = GL_FALSE;
    }
}
int Renderer::getWidth(){
    return displaymode->w;
}
int Renderer::getHeight(){
    return displaymode->h;
}
float Renderer::getAspect(){
    return (float)displaymode->w/(float)displaymode->h;
}
SDL_Window* Renderer::getWindow(){
    return window;
}
void Renderer::stop() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}