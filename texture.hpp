#pragma once
#include <cstdio>
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <inttypes.h>

/// @brief #include "texture.hpp"
///
/// Holds functions and variables necessary for creating and using a texture.
class Texture {
    public:
        /// @brief Creates a texture object.
        /// @param imagefile The file path to the image to read in.
        /// @param texture_mag_filter The filter to use when magnifying the texture. Use GL_NEAREST for nearest-neighbour interpolation, or GL_LINEAR for linear interpolation.
        /// @param texture_min_filter The filter to use when minimising the texture. Use GL_NEAREST for nearest-neighbour interpolation, or GL_LINEAR for linear interpolation.
        /// @param texture_wrap_s The repeat method to use along the horizontal (X) axis of the texture. Use GL_REPEAT to tile the texture, which is what you will want in most cases.
        /// @param texture_wrap_t The repeat method to use along the vertical (Y) axis of the texture. Use GL_REPEAT to tile the texture, which is what you will want in most cases.
        /// @param border_colour Sets the border colour to use if either texture wrap is set to GL_CLAMP_TO_EDGE. Set to nullptr if you are not using border clamping.
        Texture(const char* imagefile, GLint texture_mag_filter, GLint texture_min_filter, GLint texture_wrap_s, GLint texture_wrap_t, float* border_colour);
        /// @brief Deletes the OpenGL texture with the class instance name.
        ///
        /// Do not call this manually unless the class was allocated with placement new.
        ///
        /// If it was, there will be a statement like this with the class instance name:
        ///
        /// new(some_pointer) ClassName(args_to_create_class);
        ~Texture();
        /// @brief Gets the OpenGL handle of the texture.
        /// @return Returns the OpenGL handle of the texture.
        GLuint getTexture();
        /// @brief Activate the associated texture by binding it to a texture slot.
        /// @param slot The 0-indexed slot to put the texture in.
        ///
        /// Different platforms have a different number of slots, so check.
        ///
        /// If you're having to bind to slot 8 and above it's worth double-checking that:
        ///
        /// - The platform supports that many texture slots.
        ///
        /// - You really need that many textures and that there isn't a more efficient way to represent that information.
        void bindTexture(unsigned int slot);
        /// @brief Deactivates the associated texture.
        void unbindTexture();
    private:
        /// @brief The OpenGL handle of the texture.
        GLuint texture;
};