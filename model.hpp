#pragma once
#include <cstdio>
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <glm/glm.hpp>
#include <inttypes.h>
#include "shader.hpp"
#include "texture.hpp"

/// @brief #include "model.hpp"
///
/// Holds functions and variables necessary for creating a BasicModel.
///
/// This is the bare minimum needed to display a 3D object. More complex model types derive from this.
///
/// It contains an OpenGL shader handle and a pointer to the model-view-projection matrix.
class BasicModel {
    protected:
        /// @brief A pointer to all the floats comprising the model vertices.
        GLfloat* vertices;
        /// @brief A pointer to all the indices of the model.
        GLuint* indices;
        /// @brief The number of indices in the model.
        GLuint indexnum;
        /// @brief The number of floats in the model.
        GLuint floatnum;
        /// @brief The number of vertices in the model.
        ///
        /// This is not the same as the number of floats in the model.
        ///
        /// Vertices may have several floats each.
        GLuint vertexcount;
        /// @brief The location of the vertex shader uniform for the model-view-projection matrix.
        GLint matloc = 0;
        /// @brief The OpenGL handle of the shader associated with the model.
        GLuint shader;
        /// @brief The OpenGL handle of the vertex buffer populated with the model's vertex data.
        GLuint vertexbuffer;
        /// @brief The OpenGL handle of the index buffer populated with the model's indices.
        GLuint indexbuffer;
        /// @brief The OpenGL handle of the vertex array object.
        ///
        /// This contains information specifying the attributes of the vertex, such as XYZ position coordinates and UV texture coordinates.
        GLuint vao;
        /// @brief A pointer to the model-view-projection matrix.
        GLfloat* MVP;
        /// @brief The number of floats in a vertex.
        GLuint stride;
        /// @brief Some useful information about the model: the maximum, minimum, mean of the largest and lowest, and range values for all three coordinate axes.
        ///
        /// 2D array formatted as follows: stats[<x|y|z>][max|min|middle|size]
        GLfloat stats[3][4] = {0};
        /// @brief Repopulates the vertex buffer with the vertices.
        ///
        /// Used if the vertex data has changed, such as the model being moved or scaled.
        void regenerateVertexBuffer(GLfloat* new_vertices);
        /// @brief Prepares the model for drawing by binding the shader, any textures being used, and sets relevant uniforms.
        virtual void drawSetup();
    public:
        /// @brief Creates an instance of a BasicModel, the bare minimum needed to represent and draw a 3D object.
        /// @param vertices_in An array of float vertices.
        /// @param number_of_floats The number of floats in / lenth of vertices_in.
        /// @param indices_in An array of unsigned integer indices.
        /// @param number_of_indices The number of indices in / length of indices_in. This is not the same as the number of vertices.
        /// @param modelshader Pointer to the Shader to use. This should be a pointer to the class instance, not the shader's OpenGL handle.
        /// @param mvp Pointer to the model-view-projection matrix.
        BasicModel(GLfloat* vertices_in, GLuint number_of_floats, GLuint* indices_in, GLuint number_of_indices, Shader* modelshader, glm::mat4* mvp); 
        /// @brief Gets the maximum X, Y and Z values in the model.
        /// @return A 3-float vector containing the maximum values.
        ///
        /// You can access the individual values like this: getMax().x, getMax().y, getMax().z
        glm::vec3 getMax();
        /// @brief Gets the minimum X, Y and Z values in the model.
        /// @return A 3-float vector containing the minimum values.
        ///
        /// You can access the individual values like this: getMin().x, getMin().y, getMin().z
        glm::vec3 getMin();
        /// @brief Gets the middle X, Y and Z values in the model.
        /// @return A 3-float vector containing the middle values.
        ///
        /// You can access the individual values like this: getMiddle().x, getMiddle().y, getMiddle().z
        glm::vec3 getMiddle();
        /// @brief Gets the difference between the maximum and minimum X, Y and Z values in the model.
        /// @return A 3-float vector containing the differences.
        ///
        /// You can access the individual values like this: getSize().x, getSize().y, getSize().z
        glm::vec3 getSize();
        /// @brief Moves the model by the modifier values specified.
        /// @param x The value by which to move the model along the X axis.
        /// @param y The value by which to move the model along the Y axis.
        /// @param z The value by which to move the model along the Z axis.
        void translate(GLfloat x, GLfloat y, GLfloat z);
        /// @brief Scales the model.
        ///
        /// The model is scaled about the middle coordinate values (mean of maximum and minimum).
        /// @param scalar The scalar value by which to scale the model.
        void scaleCentre(GLfloat scalar);
        /// @brief Scales the model about the centre of the base.
        ///
        /// The model is scaled about the X and Z middle coordinate values (mean of maximum and minimum), and the minimum Y value.
        /// @param scalar The scalar value by which to scale the model.
        void scaleBase(GLfloat scalar); 
        /// @brief Draws the model to the screen.
        virtual void draw();
        /// @brief Cleans up by freeing any manually allocated memory when the class instance passes out of scope.
        ///
        /// Do not call this manually unless the class was allocated with placement new.
        ///
        /// If it was, there will be a statement like this with the class instance name:
        ///
        /// new(some_pointer) ClassName(args_to_create_class);
        virtual ~BasicModel();
};

/// @brief #include "model.hpp"
///
/// Holds functions and variables necessary for creating a SingleColourModel.
///
/// This is a type for models that are the same colour all over. They will appear as a coloured silhoutette.
///
/// It contains an OpenGL shader handle, a pointer to the model-view-projection matrix, and the colour of the model.
class SingleColourModel : public BasicModel {
    protected:
        /// @brief the shader uniform location for the model colour.
        GLint colourloc = 0;
        /// @brief An array of four floats to store the model colour.
        GLfloat colour[4];
        /// @brief Prepares the model for drawing by binding the shader, any textures being used, and sets relevant uniforms.
        void drawSetup() override;
    public:
        /// @brief Creates an instance of a SingleColourModel, a model drawn entirely in a single block colour.
        /// @param vertices_in An array of float vertices.
        /// @param number_of_floats The number of floats in / lenth of vertices_in.
        /// @param indices_in An array of unsigned integer indices.
        /// @param number_of_indices The number of indices in / length of indices_in. This is not the same as the number of vertices.
        /// @param modelshader Pointer to the Shader to use. This should be a pointer to the class instance, not the shader's OpenGL handle.
        /// @param mvp Pointer to the model-view-projection matrix.
        /// @param r The red value of the model colour. Can be between 0.0 and 1.0 inclusive.
        /// @param g The green value of the model colour. Can be between 0.0 and 1.0 inclusive.
        /// @param b The blue value of the model colour. Can be between 0.0 and 1.0 inclusive.
        /// @param a The opacity of the model. Can be between 0.0 and 1.0 inclusive.
        SingleColourModel(GLfloat* vertices_in, GLuint number_of_floats, GLuint* indices_in, GLuint number_of_indices, Shader* modelshader, glm::mat4* mvp, float r, float g, float b, float a);
        /// @brief Cleans up by freeing any manually allocated memory when the class instance passes out of scope.
        ///
        /// Do not call this manually unless the class was allocated with placement new.
        ///
        /// If it was, there will be a statement like this with the class instance name:
        ///
        /// new(some_pointer) ClassName(args_to_create_class);
        ~SingleColourModel() override;
};

/// @brief #include "model.hpp"
///
/// Holds functions and variables necessary for creating a ManyTextureModel.
///
/// This is a type for models that use several different texture objects.
///
/// It contains an OpenGL shader handle, a pointer to the model-view-projection matrix, and an array of OpenGL texture handles.
class ManyTextureModel : public BasicModel {
    protected:
        /// @brief The shader uniform location for the array of texture objects.
        GLint arrayloc = 0;
        /// @brief An array of 0-indexed numbers to label the texture instances.
        ///
        /// This has 8 spaces by default. If you need more than 8, there's probably a better model type for the asset.
        GLint samplers[8] = {};
        /// @brief An array containing the OpenGL texture handles.
        ///
        /// This has 8 spaces by default. If you need more than 8, there's probably a better model type for the asset.
        GLuint textures[8] = {};
        /// @brief The number of textures associated with the model.
        unsigned int texnum;
        /// @brief Prepares the model for drawing by binding the associated textures.
        virtual void textureSetup();
    public:
        /// @brief Creates an instance of a ManyTextureModel, a model drawn using several different texture objects.
        /// @param vertices_in An array of float vertices.
        /// @param number_of_floats The number of floats in / lenth of vertices_in.
        /// @param indices_in An array of unsigned integer indices.
        /// @param number_of_indices The number of indices in / length of indices_in. This is not the same as the number of vertices.
        /// @param texturearray An array of pointers to the Textures to use. These should be pointers to class instances, not OpenGL handles.
        /// @param number_of_textures The number of textures associated with this model. If you need more than 8, you're probably doing it wrong.
        /// @param modelshader Pointer to the Shader to use. This should be a pointer to the class instance, not the shader's OpenGL handle. 
        /// @param mvp Pointer to the model-view-projection matrix.
        ManyTextureModel(GLfloat* vertices_in, GLuint number_of_floats, GLuint* indices_in, GLuint number_of_indices, Texture** texturearray, uint8_t number_of_textures, Shader* modelshader, glm::mat4* mvp);
        /// @brief Draws the model to the screen.
        void draw() override;
        /// @brief Cleans up by freeing any manually allocated memory when the class instance passes out of scope.
        ///
        /// Do not call this manually unless the class was allocated with placement new.
        ///
        /// If it was, there will be a statement like this with the class instance name:
        ///
        /// new(some_pointer) ClassName(args_to_create_class);
        ~ManyTextureModel() override;
};

/// @brief #include "model.hpp"
///
/// Holds functions and variables necessary for creating a SingleTextureModel.
///
/// This is a type for models that use one texture for the entire model.
///
/// It contains an OpenGL shader handle, a pointer to the model-view-projection matrix, and an OpenGL texture handle.
class SingleTextureModel : public BasicModel {
    protected:
        /// @brief The shader uniform location for the texture handle.
        GLint texloc;
        /// @brief The OpenGL texture handle.
        GLuint image;
        /// @brief Prepares the model for drawing by binding the associated texture.
        virtual void textureSetup();
    public:
        /// @brief Creates an instance of a SingleTextureModel, a model drawn using just one texture object.
        /// @param vertices_in An array of float vertices.
        /// @param number_of_floats The number of floats in / lenth of vertices_in.
        /// @param indices_in An array of unsigned integer indices.
        /// @param number_of_indices The number of indices in / length of indices_in. This is not the same as the number of vertices.
        /// @param picture Pointer to the Texture to use. This should be a pointer to the class instance, not the texture's OpenGL handle.
        /// @param modelshader Pointer to the Shader to use. This should be a pointer to the class instance, not the shader's OpenGL handle. 
        /// @param mvp Pointer to the model-view-projection matrix.
        SingleTextureModel(GLfloat* vertices_in, GLuint number_of_floats, GLuint* indices_in, GLuint number_of_indices, Texture* picture, Shader* modelshader, glm::mat4* mvp);
        /// @brief Draws the model to the screen.
        void draw() override;
        /// @brief Cleans up by freeing any manually allocated memory when the class instance passes out of scope.
        ///
        /// Do not call this manually unless the class was allocated with placement new.
        ///
        /// If it was, there will be a statement like this with the class instance name:
        ///
        /// new(some_pointer) ClassName(args_to_create_class);
        ~SingleTextureModel() override;
};

/// @brief #include "model.hpp"
///
/// Holds functions and variables necessary for creating a DispModel.
///
/// This is a type for models that use a displacement map to warp the texture.
///
/// It contains an OpenGL shader handle, a pointer to the model-view-projection matrix, and pair of OpenGL texture handles.
class DispModel : public BasicModel {
    protected:
        /// @brief The shader uniform location for the array of texture objects.
        GLint arrayloc = 0;
        /// @brief An array of 0-indexed numbers to label the texture instances.
        ///
        /// This has 2 spaces by default. If you need more, you'll need to rewrite the relevant shader.
        GLint samplers[2] = {};
        /// @brief An array containing the OpenGL texture handles.
        ///
        /// This has 2 spaces by default. If you need more, you'll need to rewrite the relevant shader.
        GLuint textures[2] = {};
        /// @brief The number of textures associated with the model.
        unsigned int texnum;
        /// @brief The amount to scroll the texture on the S axis each frame.
        float scroll_x1;
        /// @brief The amount to scroll the texture on the T axis each frame.
        float scroll_y1;
        /// @brief The amount to scroll the displacement map on the S axis each frame.
        float scroll_x2;
        /// @brief The amount to scroll the displacement map on the T axis each frame.
        float scroll_y2;
        /// @brief Prepares the model for drawing by binding the associated textures.
        virtual void textureSetup();
        /// @brief Scrolls the textures by adding the scroll values to the appropriate floats in the vertex buffer.
        virtual void scroll();
    public:
        /// @brief Creates an instance of a ManyTextureModel, a model that uses a displacement map to warp the texture.
        /// @param vertices_in An array of float vertices.
        /// @param number_of_floats The number of floats in / lenth of vertices_in.
        /// @param indices_in An array of unsigned integer indices.
        /// @param number_of_indices The number of indices in / length of indices_in. This is not the same as the number of vertices. 
        /// @param texturearray An array of pointers to the Textures to use. These should be pointers to class instances, not OpenGL handles. 
        /// @param number_of_textures The number of textures associated with this model. The shader written to use with this class only functions with two textures, but in theory more could be supported.
        /// @param modelshader Pointer to the Shader to use. This should be a pointer to the class instance, not the shader's OpenGL handle. 
        /// @param mvp Pointer to the model-view-projection matrix.
        /// @param x1 The amount to scroll the texture on the S axis each frame.
        /// @param y1 The amount to scroll the texture on the T axis each frame.
        /// @param x2 The amount to scroll the displacement map on the S axis each frame.
        /// @param y2 The amount to scroll the displacement map on the T axis each frame.
        DispModel(GLfloat* vertices_in, GLuint number_of_floats, GLuint* indices_in, GLuint number_of_indices, Texture** texturearray, uint8_t number_of_textures, Shader* modelshader, glm::mat4* mvp, float x1, float y1, float x2, float y2);
        /// @brief Draws the model to the screen.
        void draw() override;
        /// @brief Cleans up by freeing any manually allocated memory when the class instance passes out of scope.
        ///
        /// Do not call this manually unless the class was allocated with placement new.
        ///
        /// If it was, there will be a statement like this with the class instance name:
        ///
        /// new(some_pointer) ClassName(args_to_create_class);
        ~DispModel() override;
};

/// @brief #include "model.hpp"
///
/// Holds functions and variables necessary for creating a SubunitModel.
///
/// This is a type for models that can repeat subunits of a texture, allowing a texture atlas to be used while only needing a single OpenGL texture object.
///
/// It contains an OpenGL shader handle, a pointer to the model-view-projection matrix, and an OpenGL texture handle.
class SubunitModel : public BasicModel {
    protected:
        /// @brief The shader uniform location for the texture handle.
        GLint texloc;
        /// @brief The OpenGL texture handle.
        GLuint image;
        /// @brief Prepares the model for drawing by binding the associated texture.
        virtual void textureSetup();
    public:
        /// @brief Creates an instance of a SubunitModel, a model that can tile sections of an image.
        /// @param vertices_in An array of float vertices.
        /// @param number_of_floats The number of floats in / lenth of vertices_in.
        /// @param indices_in An array of unsigned integer indices.
        /// @param number_of_indices The number of indices in / length of indices_in. This is not the same as the number of vertices.
        /// @param picture Pointer to the Texture to use. This should be a pointer to the class instance, not the texture's OpenGL handle.
        /// @param modelshader Pointer to the Shader to use. This should be a pointer to the class instance, not the shader's OpenGL handle. 
        /// @param mvp Pointer to the model-view-projection matrix.
        SubunitModel(GLfloat* vertices_in, GLuint number_of_floats, GLuint* indices_in, GLuint number_of_indices, Texture* picture, Shader* modelshader, glm::mat4* mvp);
        /// @brief Draws the model to the screen.
        void draw() override;
        /// @brief Cleans up by freeing any manually allocated memory when the class instance passes out of scope.
        ///
        /// Do not call this manually unless the class was allocated with placement new.
        ///
        /// If it was, there will be a statement like this with the class instance name:
        ///
        /// new(some_pointer) ClassName(args_to_create_class);
        ~SubunitModel() override;
};