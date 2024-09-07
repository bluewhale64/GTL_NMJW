#include "postbox.hpp"
#include <cmath>
#include <cstring>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stb/stb_image.h>
#include <inttypes.h>

Postbox::Postbox(GLfloat* vertices_in, GLuint number_of_floats, GLuint* indices_in, GLuint number_of_indices, Texture* picture, Shader* modelshader, glm::mat4* mvp, glm::vec3 spawn_location) : SingleTextureModel(vertices_in, number_of_floats, indices_in, number_of_indices, picture, modelshader, mvp){
    animationSpeed = 1.0;
    animationState = 0; //idle by default
    animationProgress = 0;
    position = spawn_location;
}
void Postbox::walkAnimationTransform(){
    glm::vec3 anchorPoint_1 = glm::vec3(1, 0, -10);
    glm::vec3 anchorPoint_2 = glm::vec3(1, 0, -10);
    //choose anchor points for the shoes
    //make a copy of the coordinates, apply the transformation, then give these transformed coordinates to the vertex buffer.
    //apply speed and record previous state

    //use the idle animation as a guide
}
void Postbox::idleAnimationTransform(){
    //make copy of model vertices and copy the standard vertices in
    GLfloat* transformed_vertices = (GLfloat*) malloc(floatnum * sizeof(GLfloat));
    std::memcpy(transformed_vertices, vertices, floatnum * sizeof(GLfloat));
    
    //rotate shoes, lift postbox a little
    //centres of rotation
    glm::vec3 anchorPoint_1 = glm::vec3(-0.6875,  0.0625, 0.01); //first shoe anchorpoint
    glm::vec3 anchorPoint_2 = glm::vec3(-0.1875,  0.0625, 0.02); //second shoe anchorpoint

    //set maximum angle to 22deg
    //60 can be replaced with framerate
    float framerate = 60;
    float maximumAngleDeg = 20; //degrees
    float periodFunction = 0.5 * (cos(M_PI * ((2 * animationSpeed * animationProgress) - framerate) / framerate) + 1);
    //animationProgress effectively functions as the x axis of a time graph.
    float angleRad = maximumAngleDeg * periodFunction * M_PI / 180.0f;
    float postboxRaise = periodFunction * 0.25;
    //BEWARE: you may need to make the angle negative.
    
    GLfloat x, y, new_x, new_y;
    //box body - i+0
    for(int i = 0; i < 4; i++){
        transformed_vertices[stride * (i + 0) + 1] += new_y + postboxRaise;
    }
    //first shoe - i+4
    for(int i = 0; i < 4; i++){
        //subtract anchorpoint from each coordinate in section
        x = transformed_vertices[stride * (i + 4) + 0] - anchorPoint_1.x;
        y = transformed_vertices[stride * (i + 4) + 1] - anchorPoint_1.y;
        //rotate by modulated sine
        new_x = (x * cos(angleRad)) - (y * sin(angleRad));
        new_y = (x * sin(angleRad)) + (y * cos(angleRad));
        //add anchorpoint
        transformed_vertices[stride * (i + 4) + 0] = new_x + anchorPoint_1.x;
        transformed_vertices[stride * (i + 4) + 1] = new_y + anchorPoint_1.y;
    }
    //second shoe - i+8
    for(int i = 0; i < 4; i++){
        //subtract anchorpoint from each coordinate in section
        x = transformed_vertices[stride * (i + 8) + 0] - anchorPoint_2.x;
        y = transformed_vertices[stride * (i + 8) + 1] - anchorPoint_2.y;
        //rotate by modulated sine
        new_x = (x * cos(angleRad)) - (y * sin(angleRad));
        new_y = (x * sin(angleRad)) + (y * cos(angleRad));
        //add anchorpoint
        transformed_vertices[stride * (i + 8) + 0] = new_x + anchorPoint_2.x;
        transformed_vertices[stride * (i + 8) + 1] = new_y + anchorPoint_2.y;
    }

    //add current position to all coordinates
    //stolen from underlying BasicModel::translate() to act on the transformed vertices
    //I can add a decent fix later
    for(GLuint i = 0; i < vertexcount; i++){
        transformed_vertices[stride * i + 0] += position.x;
        transformed_vertices[stride * i + 1] += position.y;
        transformed_vertices[stride * i + 2] += position.z;
    }
    //then load into the vertex buffer
    regenerateVertexBuffer(transformed_vertices);
    free(transformed_vertices); //this might be causing issues
    //mallocing and freeing a bulk of floats every frame is probably less than efficient.
    //that said, it's 2am and I just want this to work.
    animationProgress++;
    /*
    //this needs seeing to - a way to reset the animation after each loop cycle.
    if(animationProgress > framerate / animationSpeed){
        animationProgress -= framerate;
    }
    */
}
void Postbox::setAnimationState(int state){
    if(state == 0 || state == 1){
        animationState = state;
        animationProgress = 0;
    }
}
void Postbox::setAnimationSpeed(float speed){
    animationSpeed = speed;
}
void Postbox::draw(){
    
    idleAnimationTransform();

    //translate(position.x, position.y, position.z);
    //currently translate acts only on the underlying vertices
    //an alternative method must be used
    //the eventual idea is to retail a baseline copy of standard NPC vertices then edit a copy of those
    //not to edit the vertices within the model, but to instead apply a transformation
    //regenerateVertexBuffer(vertices);
    drawSetup();
    textureSetup();
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glDrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
Postbox::~Postbox(){}