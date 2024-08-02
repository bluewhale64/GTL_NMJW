#include "rubik.hpp"
#include "model.hpp"
#include "datatypes.hpp"
#include <inttypes.h>
#include <GL/gl.h>
//make float arrays for the positions
Rubik::Rubik(uint8_t* rgb6, GLfloat x, GLfloat y, GLfloat z, Texture* picture, Shader* modelshader, glm::mat4* mvp) : SingleTextureModel({-0.5f, -0.5f, -0.5f, 0.5f, 1.0f, 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f,  0.5f, 0.5f, 0.75f, 0.5f, -0.5f,  0.5f, 0.0f, 0.75f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f,  0.5f,  0.5f, 0.0f, 0.5f, -0.5f,  0.5f, -0.5f, 0.5f, 0.25f, 0.5f,  0.5f, -0.5f, 0.0f, 0.25f, -0.5f,  0.5f,  -0.5f, 1.0f, 0.5f, -0.5f, -0.5f,  -0.5f, 1.0f, 0.75f, 0.5f, -0.5f, -0.5f, 0.5f, 1.0f, 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  0.5f, 1.0f, 0.75f, 0.5f,  0.5f, -0.5f, 0.5f, 0.75f}, 70, {0,1,2,1,3,2,2,3,4,3,5,4,4,5,6,5,7,6,6,7,0,7,1,0,2,4,8,8,9,2,10,11,12,12,13,10}, 36, picture, modelshader, mvp){
    //if Rubik is initialised with nullptr argument, use default colour values
    if(rgb6 != nullptr){
        //copy 18 bytes (6 triplets of RGB 0-255 values) into the colour array
        memcpy(colours, rgb6, 18);
    }
    this->translate(x, y, z);
}
//list of squares in each path around the cube
//see notebook diagram for indices
uint8_t Rubik::paths[9][12] = {
    { 0, 7, 6, 9,16,15,36,43,42,45,52,51},
    { 1, 8, 5,10,17,14,37,44,41,46,53,50},
    {49,48,47,40,39,38,13,12,11, 4, 3, 2},
    {51,50,49,29,28,27,11,10, 9,20,19,18},
    {52,53,48,30,35,34,12,17,16,21,26,25},
    {47,46,45,24,23,22,15,14,13,33,32,31},
    { 6, 5, 4,27,34,33,38,37,36,22,21,20},
    { 7, 8, 3,28,35,32,39,44,43,23,26,19},
    {18,25,24,42,41,40,31,30,29, 2, 1, 0}};
//face each path is associated with
uint8_t Rubik::face[9] = {1,6,3,0,6,4,2,6,5};
//Model needs to generate its own texture in situ, and contain arguments to regenerate it from a pixel buffer.
void Rubik::rotateFace(uint8_t f, uint8_t d){
    //function to rotate a face when a track running around it is twisted
    //f is the face to be rotated
    //d is whether the face is rotated clockwise (0) or anticlockwise (1)
    if(d == 0){
        uint8_t store = this->squares[(9*f)+7];
        for(uint8_t i = (9 * f) + 7; i > 9 * f; i--){
            this->squares[i] = this->squares[i-1];
        }
        this->squares[9*f] = store;
    }
    else{
        uint8_t store = this->squares[9*f];
        for(uint8_t i = 9 * f; i < (9 * f) + 7; i++){
            this->squares[i] = this->squares[i+1];
        }
        this->squares[(9*f)+7] = store;
    }
}
void Rubik::rotatePath(uint8_t p, uint8_t d){
    //function to rotate a path of squares
    //p is the path to be rotated (0-8)
    //d is whether the path is rotated forwards (0) or backwards (1)
    //this function calls rotateFace - the d parameter is passed to it unmodified
    if(d == 0){
        uint8_t store0 = this->squares[paths[p][ 9]];
        uint8_t store1 = this->squares[paths[p][10]];
        uint8_t store2 = this->squares[paths[p][11]];
        for(int i = 2; i >= 0; i--){
            this->squares[paths[p][(3 * i) + 3]] = this->squares[paths[p][(3 * i) + 0]];
            this->squares[paths[p][(3 * i) + 4]] = this->squares[paths[p][(3 * i) + 1]];
            this->squares[paths[p][(3 * i) + 5]] = this->squares[paths[p][(3 * i) + 2]];
        }
        this->squares[paths[p][0]] = store0;
        this->squares[paths[p][1]] = store1;
        this->squares[paths[p][2]] = store2;
    }
    else{
        uint8_t store0 = this->squares[paths[p][0]];
        uint8_t store1 = this->squares[paths[p][1]];
        uint8_t store2 = this->squares[paths[p][2]];
        for(int i = 0; i < 3; i++){
            this->squares[paths[p][(3 * i) + 0]] = this->squares[paths[p][(3 * i) + 3]];
            this->squares[paths[p][(3 * i) + 1]] = this->squares[paths[p][(3 * i) + 4]];
            this->squares[paths[p][(3 * i) + 2]] = this->squares[paths[p][(3 * i) + 5]];
        }
        this->squares[paths[p][ 9]] = store0;
        this->squares[paths[p][10]] = store1;
        this->squares[paths[p][11]] = store2;
    }
    rotateFace(face[p], d);
}
Rubik::~Rubik(){}
