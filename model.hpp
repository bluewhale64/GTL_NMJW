#pragma once
#include <cstdio>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <inttypes.h>

class Shader {
    private:
        GLuint program = glCreateProgram();
        static unsigned int compile(const char* source, unsigned int type, const char* shadername);
        const char* parse(const char* filepath);
    public:
        Shader(const char* VertexShader, const char* FragmentShader);
        ~Shader();
        void bind();
        void unbind();
        GLuint getShader();
        GLint getUniformLocation(const char* uniformname);
        void setUniform4f(const char* uniformname, float v0, float v1, float v2, float v3);
        void setUniform1i(const char* uniformname, int i0);
        void setUniformMat4f(const char* uniformname, glm::mat4& matrix);
};

class Texture {
    public:
        Texture(const char* imagefile, GLint texture_mag_filter, GLint texture_min_filter, GLint texture_wrap_s, GLint texture_wrap_t, float* border_colour);
        ~Texture();
        GLuint getTexture();
        void bindTexture(unsigned int slot);
        void unbindTexture(void);
    private:
        GLuint texture;
};

class BasicModel {
    protected:
        GLfloat* vertices;
        GLuint* indices;
        GLuint indexnum;
        GLuint floatnum;
        GLuint vertexcount;
        GLint matloc = 0;
        GLuint shader;
        GLuint vertexbuffer;
        GLuint indexbuffer;
        GLuint vao;
        GLfloat* MVP;
        GLuint stride;
        GLfloat stats[3][4] = {0}; //xyz by max,min,middle,size
        void regenerateVertexBuffer();
        virtual void drawSetup();
    public:
        BasicModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Shader* modelshader, glm::mat4* mvp); 
        glm::vec3 getMax();
        glm::vec3 getMin();
        glm::vec3 getMiddle();
        glm::vec3 getSize();
        void translate(GLfloat x, GLfloat y, GLfloat z);
        void scaleCentre(GLfloat scalar);
        void scaleBase(GLfloat scalar); 
        virtual void draw();
        virtual ~BasicModel();
};

class SingleColourModel : public BasicModel {
    protected:
        GLint colourloc = 0;
        GLfloat colour[4];
        void drawSetup() override;
    public:
        SingleColourModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Shader* modelshader, glm::mat4* mvp, float r, float g, float b, float a);
        ~SingleColourModel() override;
};

class ManyTextureModel : public BasicModel {
    protected:
        GLint arrayloc = 0;
        GLint samplers[8] = {};
        GLuint textures[8] = {};
        unsigned int texnum;
        virtual void textureSetup();
    public:
        ManyTextureModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Texture** texturearray, uint8_t number_of_textures, Shader* modelshader, glm::mat4* mvp);
        void draw() override;
        ~ManyTextureModel() override;
};

class SingleTextureModel : public BasicModel {
    protected:
        GLint texloc;
        GLuint image;
        GLint arrayloc = 0;
        unsigned int texnum;
        virtual void textureSetup();
    public:
        SingleTextureModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Texture* picture, Shader* modelshader, glm::mat4* mvp);
        void draw() override;
        ~SingleTextureModel() override;
};

class DispModel : public BasicModel {
    protected:
        GLint arrayloc = 0;
        GLint samplers[8] = {};
        GLuint textures[8] = {};
        unsigned int texnum;
        float scroll_x1;
        float scroll_y1;
        float scroll_x2;
        float scroll_y2;
        virtual void textureSetup();
        virtual void scroll();
    public:
        DispModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Texture** texturearray, uint8_t number_of_textures, Shader* modelshader, glm::mat4* mvp, float x1, float y1, float x2, float y2);
        void draw() override;
        ~DispModel() override;
};

class SubunitModel : public BasicModel {
    protected:
        GLint texloc;
        GLuint image;
        virtual void textureSetup();
    public:
        SubunitModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Texture* picture, Shader* modelshader, glm::mat4* mvp);
        void draw() override;
        ~SubunitModel() override;
};
