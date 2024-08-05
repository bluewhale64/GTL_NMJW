#pragma once
#include <GL/gl.h>
#include <inttypes.h>
#include <cstdio>

/// @brief #include "loader.hpp"
///
/// Holds the functions necessary for asset loading.
///
/// The asset formats are specified by a 4-byte sequence near the start of the file.
///
/// The format names may seem a bit arbitrary - this is a work in progress and will be better labelled in the future.
namespace Loader {
        /// @brief Loads an asset.
        ///
        /// Based on the header of the asset file, the loader will internally detect the asset format and load it appropriately.
        ///
        /// If the format is unrecognised, this will return -1 and print an error message.
        /// @param filename A string containing the path to the asset file.
        /// @return 0 if the asset was loaded successfully, and -1 if it wasn't.
        int load(const char* filename); 
        /// @brief Gets the size in bytes of the float type used by the most recently loaded asset.
        /// @return The size in bytes of the asset's floats.
        uint16_t getFloatSize();
        /// @brief Gets the size in bytes of indices used by the most recently loaded asset.
        /// @return The size in bytes of the asset's vertex indices.
        uint16_t getIndexSize();
        /// @brief Gets the number of floats in the most recently loaded asset.
        /// @return The number of floats in the most recently loaded asset.
        uint32_t getFloatCount();
        /// @brief Gets the number of indices in the most recently loaded asset.
        /// @return The number of indices in the most recently loaded asset.
        uint32_t getIndexCount();
        /// @brief Gets a pointer to an array of floats from the most recently loaded asset.
        /// @return A pointer to an array of floats from the most recently loaded asset.
        GLfloat* getVertexBuffer();
        /// @brief Gets a pointer to an array of indices from the most recently loaded asset.
        /// @return A pointer to an array of indices from the most recently loaded asset.
        GLuint* getIndexBuffer();
        /// @brief Frees the memory held by the asset loading buffers.
        ///
        /// Call it at the end of the program.
        void stop();
};