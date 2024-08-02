#pragma once
#include <GL/gl.h>
#include <inttypes.h>
#include <cstdio>

namespace Loader {
        void init();
        int load_0000(FILE* file);
        int load(const char* filename); 
        uint16_t getFloatSize();
        uint16_t getIndexSize();
        uint32_t getFloatCount();
        uint32_t getIndexCount();
        GLfloat* getVertexBuffer();
        GLuint* getIndexBuffer();
        void stop();
};