#pragma once
#include <cstdio>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <inttypes.h>
#include "model.hpp"

class Postbox : public SingleTextureModel {
    private:
        glm::vec3 position;
        int animationState; //0 for idle, 1 for walk
        float animationSpeed; //how fast to cycle through the animation - default 1.0 = 1 loop per 60 frames
        int animationProgress; //check where the animation was on the previous frame
        void walkAnimationTransform();
        void idleAnimationTransform();
    public:
        Postbox(GLfloat* vertices_in, GLuint number_of_floats, GLuint* indices_in, GLuint number_of_indices, Texture* picture, Shader* modelshader, glm::mat4* mvp, glm::vec3 spawn_location);
        void setAnimationState(int state);
        void setAnimationSpeed(float speed);
        void draw() override;
        ~Postbox() override;
};
