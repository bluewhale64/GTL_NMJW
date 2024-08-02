#pragma once
#include "model.hpp"
#include "datatypes.hpp"
#include <GL/gl.h>

class Rubik : public SingleTextureModel {
    private:
        //list of squares in each path around the cube
        //see notebook diagram for indices
        static uint8_t paths[9][12];
        //face each path is associated with
        static uint8_t face[9];
        //default side colours (white, red, blue, green, orange, purple)
        uint8_t colours[6][3] = {
            {255,255,255},
            {255,  0,  0},
            {  0,  0,255},
            {  0,255,  0},
            {255,127,  0},
            {255,  0,255}};
        //colour (0-5) of each square on the cube
        uint8_t squares[54] = {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5};
    public:
        //Model needs to generate its own texture in situ, and contain arguments to regenerate it from a pixel buffer.
        Rubik(uint8_t* rgb6, GLfloat x, GLfloat y, GLfloat z, Texture* picture, Shader* modelshader, glm::mat4* mvp);
        void rotateFace(uint8_t f, uint8_t d);
        void rotatePath(uint8_t p, uint8_t d);
        ~Rubik();
};