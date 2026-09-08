#pragma once
#include <cstdio>
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <glm/glm.hpp>
#include <inttypes.h>

using namespace glm;

class Animation {
    Animation();
    ~Animation();
};

//rotations need an anchor point each, or a flag to instead use coordinates relative to model.
struct Transformation {
    vec3 position = vec3(0.0f,0.0f,0.0f);
    vec3 rotation = vec3(0.0f,0.0f,0.0f);
    GLfloat scale = 1.0f;
};