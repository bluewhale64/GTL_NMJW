#include "loader.hpp"
#include <GL/gl.h>
#include <inttypes.h>
#include <cstdio>
#include <cstdlib>
#include <stdarg.h>
#include <cstring>

/// @brief This anonymous (unnamed) namespace holds the type sizes and raw data of loaded assets.
///
/// It can't be accessed by functions written outsie of this file (loader.cpp).
///
/// The functions here can access the data and pass it to code outside this file.
namespace {
    /// @brief A 12 byte sequence to test if assets being loaded are of the arbitrary asset format.
    const char footprint[13] = "NMJW_MDLFMT_";
    /// @brief A pointer to hold loaded floats from assets.
    ///
    /// Initialised as a nullptr.
    GLfloat* vertexbuffer = nullptr;
    /// @brief A pointer to hold loaded indices from assets.
    ///
    /// Initialised as a nullptr.
    GLuint* indexbuffer = nullptr;
    /// @brief A four byte stamp representing the asset type.
    uint32_t MODEL_TYPE;
    /// @brief A four byte unsigned integer holding the size in bytes of the asset.
    uint32_t FILE_SIZE;
    /// @brief A four byte unsigned integer holding the number of floats in the asset.
    uint32_t NUMBER_OF_FLOATS;
    /// @brief A four byte unsigned integer holding the size in bytes of the float type used by the asset.
    uint16_t FLOAT_SIZE;
    /// @brief A four byte unsigned integer holding the distance in bytes from the start of the file to the beginning of the list of floats.
    uint32_t FLOAT_START_OFFSET;
    /// @brief A four byte unsigned integer holding the number of indices in the asset.
    uint32_t NUMBER_OF_INDICES;
    /// @brief A four byte unsigned integer holding the size in bytes of the index type used by the asset.
    uint16_t INDEX_SIZE;
    /// @brief A four byte unsigned integer holding the distance in bytes from the start of the file to the beginning of the list of indices.
    uint32_t INDEX_START_OFFSET;
    /// @brief Prints a formatted error message describing why the asset failed to load properly.
    /// @param file A string holding the path to the asset file.
    /// @param errormessage The error message generated from the load function used on the asset.
    ///
    /// The error messgae is formatted like printf and printed to the console.
    /// @return -1 to show that an asset failed to load properly.
    [[gnu::format(printf, 2, 3)]] int loadfail(FILE* file, const char* errormessage, ...){
        va_list args;
        va_start(args, errormessage);
        std::vfprintf(stderr, errormessage, args);
        va_end(args);
        free(vertexbuffer);
        free(indexbuffer);
        if(file != nullptr){
            fclose(file);
        }
        return -1;
    }
    int load_0000(FILE* file){
        fseek(file, 20, SEEK_SET);
        fread(&NUMBER_OF_FLOATS, 4, 1, file);
        fread(&FLOAT_SIZE, 2, 1, file);
        fread(&FLOAT_START_OFFSET, 4, 1, file);
        fread(&NUMBER_OF_INDICES, 4, 1, file);
        fread(&INDEX_SIZE, 2, 1, file);
        fread(&INDEX_START_OFFSET, 4, 1, file); 
        vertexbuffer = (GLfloat*) malloc(NUMBER_OF_FLOATS  * 4);
        if(vertexbuffer == nullptr){
            return loadfail(file, "ERROR: Vertex reading buffer could not be allocated memory.\n");
        }
        fseek(file, FLOAT_START_OFFSET, SEEK_SET);
        if(FLOAT_SIZE == 8){
            double store;
            for(int i = 0; i < NUMBER_OF_FLOATS; i++){
                fread(&store, FLOAT_SIZE, 1, file);
                vertexbuffer[i] = (GLfloat)store;
            }
        }
        else if(FLOAT_SIZE == 4){
            fread(vertexbuffer, FLOAT_SIZE, NUMBER_OF_FLOATS, file);
        }
        else{
            return loadfail(file, "ERROR: Invalid float size.\n");
        }
        fseek(file, INDEX_START_OFFSET, SEEK_SET);
        if(INDEX_SIZE == 2){
            indexbuffer  = (GLuint*) calloc(NUMBER_OF_INDICES, 4);
            if(indexbuffer == nullptr){
                return loadfail(file, "ERROR: Index reading buffer could not be allocated memory.\n");
            }
            for(int i = 0; i < NUMBER_OF_INDICES; i++){
                fread(&indexbuffer[i], INDEX_SIZE, 1, file);
            }
        }
        else if(INDEX_SIZE == 4){
            indexbuffer  = (GLuint*) malloc(NUMBER_OF_INDICES * 4);
            if(indexbuffer == nullptr){
                return loadfail(file, "ERROR: Index reading buffer could not be allocated memory.\n");
            }
            fread(indexbuffer, INDEX_SIZE, NUMBER_OF_INDICES, file);
        }
        else{
            return loadfail(file, "ERROR: Invalid index size.\n");
        }
        fclose(file);
        return 0;
    }
}
int Loader::load(const char* filename){
    FILE* f = fopen(filename, "rb");
    if(f == nullptr){
        return loadfail(f, "ERROR: %s file could not be found or opened.\n", filename);
    }
    char test_footprint[12];
    fread(test_footprint, 1, 12, f);
    if(std::memcmp(footprint, test_footprint, 12) != 0){
        return loadfail(f, "ERROR: %s file format unrecognised.\n", filename);
    }
    fread(&MODEL_TYPE, 4, 1, f);
    fread(&FILE_SIZE, 4, 1, f);
    std::printf("Read contents of file %s (%d bytes)\n", filename, FILE_SIZE);
    if(MODEL_TYPE == 0b0000){
        return load_0000(f);
    }
    else{
        return loadfail(f, "ERROR: %s model format unrecognised.\n", filename);
    }
} 
uint16_t Loader::getFloatSize(){
    return FLOAT_SIZE;
}
uint16_t Loader::getIndexSize(){
    return INDEX_SIZE;
}
uint32_t Loader::getFloatCount(){
    return NUMBER_OF_FLOATS;
}
uint32_t Loader::getIndexCount(){
    return NUMBER_OF_INDICES;
}
GLfloat* Loader::getVertexBuffer(){
    return vertexbuffer;
}
GLuint* Loader::getIndexBuffer(){
    return indexbuffer;
}
void Loader::stop(){
    free(vertexbuffer);
    free(indexbuffer);
}