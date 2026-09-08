#pragma once
#include <cstdio>
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <glm/glm.hpp>
#include <inttypes.h>

using namespace glm;

/// @brief #include "shader.hpp"
///
/// Holds functions and variables necessary for creating and using a GLSL shader.
class Shader {
    private:
        /// @brief The OpenGL handle of the shader.
        GLuint program = glCreateProgram();
        /// @brief Compiles the GLSL code into a functional shader.
        /// @param source A string containg the source code to compile.
        /// @param type The type of the shader. GL_VERTEX_SHADER or GL_FRAGMENT_SHADER (other shaders are available).
        /// @param shadername The name of the shader. Used for error messages if the shader fails to compile.
        /// @return An identifier handle to the shader object.
        static unsigned int compile(const char* source, unsigned int type, const char* shadername);
        /// @brief Reads in GLSL source code from a file.
        /// @param filepath The file path to the shader to read in.
        /// @return A string containing the loaded shader code.
        const char* parse(const char* filepath);
    public:
        /// @brief Creates a shader object consisting of a vertex and fragment shader.
        /// @param VertexShader The file path to the vertex shader to read in.
        /// @param FragmentShader The file path to the fragment shader to read in.
        Shader(const char* VertexShader, const char* FragmentShader);
        /// @brief Deletes the OpenGL shader with the class instance name.
        ///
        /// Do not call this manually unless the class was allocated with placement new.
        ///
        /// If it was, there will be a statement like this with the class instance name:
        ///
        /// new(some_pointer) ClassName(args_to_create_class);
        ~Shader();
        /// @brief Activates the shader with the class instance name.
        void bind();
        /// @brief Deactivates the currently bound shader.
        void unbind();
        /// @brief Gets the OpenGL handle of the shader.
        /// @return Returns the OpenGL handle of the shader.
        GLuint getShader();
        /// @brief Gets the uniform ID from the associated shader.
        /// @param uniformname The name of the uniform as a string.
        /// @return The uniform ID, or -1 if the uniform could not be found.
        GLint getUniformLocation(const char* uniformname);
        /// @brief Sets a uniform consisting of 4 floats to the specified values.
        /// @param uniformname The name of the uniform as a string.
        /// @param v0 The value to set at index 0.
        /// @param v1 The value to set at index 1.
        /// @param v2 The value to set at index 2.
        /// @param v3 The value to set at index 3.
        void setUniform4f(const char* uniformname, float v0, float v1, float v2, float v3);
        /// @brief Sets a uniform consisting of 1 integer to the specified value.
        /// @param uniformname The name of the uniform as a string.
        /// @param i0 The value to set.
        void setUniform1i(const char* uniformname, int i0);
        /// @brief Sets a uniform consisting of a matrix to the specified values.
        /// @param uniformname The name of the uniform as a string.
        /// @param matrix The matrix to set as the value.
        void setUniformMat4f(const char* uniformname, mat4& matrix);
};