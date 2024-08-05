#pragma once
#include "model.hpp"
#include "renderer.hpp"
#include <GL/gl.h>

/// @brief #include "rubik.hpp"
///
/// Holds functions and variables necessary for creating a Rubik's Cube.
///
/// This class is a work-in-progress, so don't use it yet!
///
/// A diagram ilustrating the square indices is available.
///
/// It contains arrays representing which squares on the cube surface are included in each possible turn path.
class Rubik : public SingleTextureModel {
    private:
        /// @brief A 2D array of the squares in each path around the cube.
        ///
        /// The format is as follows: paths[which_route][the_index_of_the_12_squares_in_that_route].
        ///
        /// This is the same for every instance of a Rubik's Cube.
        static uint8_t paths[9][12];
        /// @brief The face each path is associated with.
        ///
        /// When a path (that isn't a middle one) is twisted, a face (side) of the cube will be rotated with it.
        ///
        /// This is the same for every instance of a Rubik's Cube.
        static uint8_t face[9];
        /// @brief A 2D array of the default side colours (white, red, blue, green, orange, purple).
        ///
        /// These can be customised when the Rubik's Cube is instantiated.
        uint8_t colours[6][3] = {
            {255,255,255},
            {255,  0,  0},
            {  0,  0,255},
            {  0,255,  0},
            {255,127,  0},
            {255,  0,255}};
        /// @brief The colour (0-5) of each square on the cube.
        ///
        /// The value of each square's index indicates which colour in the colours array the square currently is.
        uint8_t squares[54] = {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5};
    public:
        /// @brief Creates an instance of a Rubik, a Rubik's Cube type.
        ///
        /// Still to do: make the class generate its texture / pixel buffer in situ based on the colours of each square.
        /// @param rgb6 Array of 6 RGB unsigned 1-byte integer triples to use for custom colours. Use nullptr instead for the default colours.
        /// @param x The desired X position of the centre of the Rubik's Cube. By default, the Rubik's Cube appears around the origin.
        /// @param y The desired Y position of the centre of the Rubik's Cube. By default, the Rubik's Cube appears around the origin.
        /// @param z The desired Z position of the centre of the Rubik's Cube. By default, the Rubik's Cube appears around the origin.
        /// @param picture Pointer to the Texture to use. This should be a pointer to the class instance, not the texture's OpenGL handle.
        /// @param modelshader Pointer to the Shader to use. This should be a pointer to the class instance, not the shader's OpenGL handle.
        /// @param mvp Pointer to the model-view-projection matrix.
        Rubik(uint8_t* rgb6, GLfloat x, GLfloat y, GLfloat z, Texture* picture, Shader* modelshader, glm::mat4* mvp);
        /// @brief Rotate a side of the Rubik's Cube by 90 degrees.
        /// @param f The face (0-5) to be rotated.
        /// @param d The direction the face is rotated. 0 for clockwise and 1 for anti-clockwise.
        void rotateFace(uint8_t f, uint8_t d);
        /// @brief Rotate a path of squares by one twist.
        ///
        /// This function calls rotateFace - the d parameter is passed to it unmodified.
        /// @param p The path (0-8) to be rotated.
        /// @param d The direction the path is rotated. 0 for forwards and 1 for backwards.
        void rotatePath(uint8_t p, uint8_t d);
        /// @brief Cleans up by freeing any manually allocated memory when the class instance passes out of scope.
        ///
        /// Do not call this manually unless the class was allocated with placement new.
        ///
        /// If it was, there will be a statement like this with the class instance name:
        ///
        /// new(some_pointer) ClassName(args_to_create_class);
        ~Rubik();
};