#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC

//To do: Squash it into 2D mode

//Orthographic matrix

#include <std_image.h>
#include <glew.h>
#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "buffer.hpp"
#include "datatypes.hpp"
#include "controls.hpp"

int main(void) {

    int width = 1080;
    int height = 720;
    float aspect = (float)width / (float)height;

    Renderer render(width, height, "Tube Simulator", nullptr, nullptr, 1, 0.0f, 0.3f, 0.2f, 0.0f);
    if (render.window == nullptr) {
        printf("Window Startup Error\n");
        return -1;
    }
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 P = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    // Camera matrix
    glm::mat4 V = glm::lookAt(
        glm::vec3(0, 0.41, 1.5), // Camera is at (10,1.5,-1.5), in World Space
        glm::vec3(-15, 0.41, 1.5), // and looks this position (0, 1.5, -1.5)
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 M = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = P * V * M; // Remember, matrix multiplication is the other way around


   
    float platver[96] = {
       -5.0, -1.0,  0.0,  0.0,  0.000,  0,   //0a'
        5.0, -1.0,  0.0, 10.0,  0.000,  0,   //1a
       -5.0,  0.0,  0.0,  0.0,  0.125,  0,   //2b'
        5.0,  0.0,  0.0, 10.0,  0.125,  0,   //3b
       -5.0,  0.0, -1.0,  0.0,  0.250,  0,   //4c'
        5.0,  0.0, -1.0, 10.0,  0.250,  0,   //5c
       -5.0, -1.0, -1.0,  0.0,  0.375,  0,   //6d
        5.0, -1.0, -1.0, 10.0,  0.375,  0,   //7d'
       -5.0, -1.0, -2.0,  0.0,  0.500,  0,   //8e'
        5.0, -1.0, -2.0, 10.0,  0.500,  0,   //9e
       -5.0,  2.5, -2.0,  0.0,  0.625,  0,  //10f'
        5.0,  2.5, -2.0, 10.0,  0.625,  0,  //11f
       -5.0,  2.5,  0.0,  0.0,  0.750,  0,  //12g'
        5.0,  2.5,  0.0, 10.0,  0.750,  0,  //13g
       -5.0,  0.0,  0.0,  0.0,  0.875,  0,  //14h'
        5.0,  0.0,  0.0, 10.0,  0.875,  0   //15h
       
    };

    unsigned int platind[42] = {
        0, 1, 3, 3, 2, 0,        //1
        2, 3, 5, 5, 4, 2,        //2
        4, 5, 7, 7, 6, 4,        //3
        6, 7, 9, 9, 8, 6,        //4
        8, 9, 11, 11, 10, 8,     //5
        10, 11, 13, 13, 12, 10,  //6
        12, 13, 15, 15, 14, 12   //7
    };

    float tubecorners[252] = {
        00.0, 0.0000, 0.0000, 0.0, 0.0, -1,
        00.0, 0.8550, 0.0000, 0.0, 0.0, -1,
        00.0, 1.7397, 0.1334, 0.0, 0.0, -1,
        00.0, 1.8417, 0.1648, 0.0, 0.0, -1,
        00.0, 1.9302, 0.2275, 0.0, 0.0, -1,
        00.0, 2.0104, 0.3050, 0.0, 0.0, -1,
        00.0, 2.1466, 0.4817, 0.0, 0.0, -1,
        00.0, 2.2557, 0.6763, 0.0, 0.0, -1,
        00.0, 2.3353, 0.8847, 0.0, 0.0, -1,
        00.0, 2.3837, 1.1025, 0.0, 0.0, -1,
        00.0, 2.4000, 1.3250, 0.0, 0.0, -1,
        00.0, 2.3837, 1.5475, 0.0, 0.0, -1,
        00.0, 2.3353, 1.7653, 0.0, 0.0, -1,
        00.0, 2.2557, 1.9737, 0.0, 0.0, -1,
        00.0, 2.1466, 2.1683, 0.0, 0.0, -1,
        00.0, 2.0104, 2.3450, 0.0, 0.0, -1,
        00.0, 1.9302, 2.4225, 0.0, 0.0, -1,
        00.0, 1.8417, 2.4852, 0.0, 0.0, -1,
        00.0, 1.7397, 2.5166, 0.0, 0.0, -1,
        00.0, 0.8550, 2.6500, 0.0, 0.0, -1,
        00.0, 0.0000, 2.6500, 0.0, 0.0, -1,
        12.5, 0.0000, 0.0000, 0.0, 0.0, -1,
        12.5, 0.8550, 0.0000, 0.0, 0.0, -1,
        12.5, 1.7397, 0.1334, 0.0, 0.0, -1,
        12.5, 1.8417, 0.1648, 0.0, 0.0, -1,
        12.5, 1.9302, 0.2275, 0.0, 0.0, -1,
        12.5, 2.0104, 0.3050, 0.0, 0.0, -1,
        12.5, 2.1466, 0.4817, 0.0, 0.0, -1,
        12.5, 2.2557, 0.6763, 0.0, 0.0, -1,
        12.5, 2.3353, 0.8847, 0.0, 0.0, -1,
        12.5, 2.3837, 1.1025, 0.0, 0.0, -1,
        12.5, 2.4000, 1.3250, 0.0, 0.0, -1,
        12.5, 2.3837, 1.5475, 0.0, 0.0, -1,
        12.5, 2.3353, 1.7653, 0.0, 0.0, -1,
        12.5, 2.2557, 1.9737, 0.0, 0.0, -1,
        12.5, 2.1466, 2.1683, 0.0, 0.0, -1,
        12.5, 2.0104, 2.3450, 0.0, 0.0, -1,
        12.5, 1.9302, 2.4225, 0.0, 0.0, -1,
        12.5, 1.8417, 2.4852, 0.0, 0.0, -1,
        12.5, 1.7397, 2.5166, 0.0, 0.0, -1,
        12.5, 0.8550, 2.6500, 0.0, 0.0, -1,
        12.5, 0.0000, 2.6500, 0.0, 0.0, -1};

    
    unsigned int tubeindices[240] = {};
    unsigned int j = 0;
    for (int i = 0; i < 21; i++) {
        if (i < 19) {
            tubeindices[j++] = 0;
            tubeindices[j++] = i + 2;
            tubeindices[j++] = i + 1;
            tubeindices[j++] = 21;
            tubeindices[j++] = i + 22;
            tubeindices[j++] = i + 23;
        }
        tubeindices[j++] = i;
        tubeindices[j++] = (i + 1 % 21) + 21;
        tubeindices[j++] = i + 21;
        tubeindices[j++] = (i + 1 % 21) + 21;
        tubeindices[j++] = i;
        tubeindices[j++] = (i + 1 % 21);
    }

    float skcorners[396] = {
        -10, 0.0000, 3.3750, 0.0, 0.0, -1, // A'
        -10,-0.5023, 3.3750, 0.0, 0.0, -1, // B'
        -10,-0.5023, 0.5925, 0.0, 0.0, -1, // C'
        -10, 0.0000, 0.2569, 0.0, 0.0, -1, // D'
        -10, 1.2916, 0.0000, 0.0, 0.0, -1, // E'
        -10, 2.5831, 0.2569, 0.0, 0.0, -1, // F'
        -10, 3.6780, 0.9885, 0.0, 0.0, -1, // G'
        -10, 4.4097, 2.0834, 0.0, 0.0, -1, // H'
        -10, 4.6666, 3.3750, 0.0, 0.0, -1, // I'
        -10, 4.4097, 4.6666, 0.0, 0.0, -1, // J'
        -10, 3.6780, 5.7615, 0.0, 0.0, -1, // K'
        -10, 2.5831, 6.4931, 0.0, 0.0, -1, // L'
        -10, 0.0000, 6.4931, 0.0, 0.0, -1, // M'
        -10,-2.0000, 6.4931, 0.0, 0.0, -1, // N'
        -10, 0.8377, 3.3750, 0.0, 0.0, -1, // b'
        -10, 1.7667, 3.2350, 0.0, 0.0, -1, // c'
        -10, 1.8738, 3.2019, 0.0, 0.0, -1, // d'
        -10, 1.9667, 3.1361, 0.0, 0.0, -1, // e'
        -10, 2.0510, 3.0547, 0.0, 0.0, -1, // f'
        -10, 2.1940, 2.8692, 0.0, 0.0, -1, // g'
        -10, 2.3085, 2.6649, 0.0, 0.0, -1, // h'
        -10, 2.3921, 2.4460, 0.0, 0.0, -1, // i'
        -10, 2.4429, 2.2174, 0.0, 0.0, -1, // j'
        -10, 2.4600, 1.9837, 0.0, 0.0, -1, // k'
        -10, 2.4429, 1.7501, 0.0, 0.0, -1, // l'
        -10, 2.3921, 1.5215, 0.0, 0.0, -1, // m'
        -10, 2.3085, 1.3026, 0.0, 0.0, -1, // n'
        -10, 2.1940, 1.0983, 0.0, 0.0, -1, // o'
        -10, 2.0510, 0.9128, 0.0, 0.0, -1, // p'
        -10, 1.9667, 0.8314, 0.0, 0.0, -1, // q'
        -10, 1.8738, 0.7656, 0.0, 0.0, -1, // r'
        -10, 1.7667, 0.7325, 0.0, 0.0, -1, // s'
        -10, 0.8378, 0.5925, 0.0, 0.0, -1, // t'
          
         10, 0.0000, 3.3750, 0.0, 0.0, -1, // A
         10,-0.5023, 3.3750, 0.0, 0.0, -1, // B
         10,-0.5023, 0.5925, 0.0, 0.0, -1, // C
         10, 0.0000, 0.2569, 0.0, 0.0, -1, // D
         10, 1.2916, 0.0000, 0.0, 0.0, -1, // E
         10, 2.5831, 0.2569, 0.0, 0.0, -1, // F
         10, 3.6780, 0.9885, 0.0, 0.0, -1, // G
         10, 4.4097, 2.0834, 0.0, 0.0, -1, // H
         10, 4.6666, 3.3750, 0.0, 0.0, -1, // I
         10, 4.4097, 4.6666, 0.0, 0.0, -1, // J
         10, 3.6780, 5.7615, 0.0, 0.0, -1, // K
         10, 2.5831, 6.4931, 0.0, 0.0, -1, // L
         10, 0.0000, 6.4931, 0.0, 0.0, -1, // M
         10,-2.0000, 6.4931, 0.0, 0.0, -1, // N
         10, 0.8377, 3.3750, 0.0, 0.0, -1, // b
         10, 1.7667, 3.2350, 0.0, 0.0, -1, // c
         10, 1.8738, 3.2019, 0.0, 0.0, -1, // d
         10, 1.9667, 3.1361, 0.0, 0.0, -1, // e
         10, 2.0510, 3.0547, 0.0, 0.0, -1, // f
         10, 2.1940, 2.8692, 0.0, 0.0, -1, // g
         10, 2.3085, 2.6649, 0.0, 0.0, -1, // h
         10, 2.3921, 2.4460, 0.0, 0.0, -1, // i
         10, 2.4429, 2.2174, 0.0, 0.0, -1, // j
         10, 2.4600, 1.9837, 0.0, 0.0, -1, // k
         10, 2.4429, 1.7501, 0.0, 0.0, -1, // l
         10, 2.3921, 1.5215, 0.0, 0.0, -1, // m
         10, 2.3085, 1.3026, 0.0, 0.0, -1, // n
         10, 2.1940, 1.0983, 0.0, 0.0, -1, // o
         10, 2.0510, 0.9128, 0.0, 0.0, -1, // p
         10, 1.9667, 0.8314, 0.0, 0.0, -1, // q
         10, 1.8738, 0.7656, 0.0, 0.0, -1, // r
         10, 1.7667, 0.7325, 0.0, 0.0, -1, // s
         10, 0.8378, 0.5925, 0.0, 0.0, -1  // t
    
    };
    unsigned int skindices[258] = {
        45,47,33,
        47,45,44,
        47,44,43,
        47,43,42,
        47,42,41,
        41,48,47,
        41,49,48,
        41,50,49,
        41,51,50,
        41,52,51,
        41,53,52,
        41,54,53,
        41,55,54,
        40,55,41,
        40,56,55,
        40,57,56,
        40,58,57,
        40,59,58,
        39,59,40,
        39,60,59,
        39,38,60,
        38,61,60,
        38,62,61,
        38,63,62,
        38,37,63,
        37,64,63,
        37,65,64,
        37,36,65,
        36,35,65,
        12, 0,14,
        14,11,12,
        14,10,11,
        14, 9,10,
        14, 8, 9,
         8,14,15,
         8,15,16,
         8,16,17,
         8,17,18,
         8,18,19,
         8,19,20,
         8,20,21,
         8,21,22,
         7, 8,22,
         7,22,23,
         7,23,24,
         7,24,25,
         7,25,26,
         6, 7,26,
         6,26,27,
         6,27, 5,
         5,27,28,
         5,28,29,
         5,29,30,
         5,30, 4,
         4,30,31,
         4,31,32,
         4,32, 3,
         3,32, 2,
        12,33, 0,
        33,12,45,
         0,34, 1,
        34, 0,33,
         1,35, 2,
        35, 1,34,
         2,36, 3,
        36, 2,35,
         3,37, 4,
        37, 3,36,
         4,38, 5,
        38, 4,37,
         5,39, 6,
        39, 5,38,
         6,40, 7,
        40, 6,39,
         7,41, 8,
        41, 7,40,
         8,42, 9,
        42, 8,41,
         9,43,10,
        43, 9,42,
        10,44,11,
        44,10,43,
        11,45,12,
        45,11,44,
        12,46,45,
        46,12,13
    };


    //Texture skplatform("textures/256x256_southkensingtonplatform.png", GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT);
    //Texture mii("textures/512x512_mii.png", GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
    //Texture moq("textures/64x64_moquette.png", GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT);
    Texture ground("textures/256x256_southkensingtonground.png", GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT);
    //Texture* textures[4] = { &skplatform, &mii, &moq, &ground };
    
    Shader textureshader("shaders/texture.vertex", "shaders/texture.fragment");
    Texture SouthKen0("textures/256x2048_southken0.png", GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT);
    Texture* tex[1] = { &SouthKen0 };
    Texture* tet[1] = { &ground };
    Model plat(platver, 96, platind, 42, tex, 1, &textureshader, &mvp);
    Model sk(skcorners, 396, skindices, 258, tet, 1, &textureshader, &mvp);
    Model tube(tubecorners, 252, tubeindices, 240, tet, 1, &textureshader, &mvp);


    while (glfwGetKey(render.window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(render.window) == 0) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        //glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        //This is done in the model class

        //plat.draw();
        //tube.draw();
        sk.draw();
        glfwSwapBuffers(render.window);
        glfwPollEvents();
        computeMatricesFromInputs(render.window, width, height, aspect);
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;      
    }

    return 0;
}