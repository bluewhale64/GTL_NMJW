class BasicModel {
    protected:
        float* vertices;
        unsigned int* indices;
        unsigned int indexnum;
        GLint matloc = 0;
        GLuint shader;
        GLuint vertexbuffer;
        GLuint indexbuffer;
        GLuint vao;
        GLfloat* MVP;
        virtual void drawsetup(){
            glUseProgram(shader);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
            glUniformMatrix4fv(matloc, 1, GL_FALSE, MVP);
        }
    public:
        BasicModel(float* vertices_in, int16_t number_of_vertices, unsigned int* indices_in, uint16_t number_of_indices, Shader* modelshader, glm::mat4* mvp) {
            shader = modelshader->getshader();
            matloc = glGetUniformLocation(shader, "MVP");
            MVP = (GLfloat*)mvp;
            vertices = vertices_in;
            indices = indices_in;
            indexnum = number_of_indices;
            glGenBuffers(1, &vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, number_of_vertices * sizeof(float), vertices, GL_STATIC_DRAW);
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glGenBuffers(1, &indexbuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_of_indices * sizeof(float), indices, GL_STATIC_DRAW);
        } 
        virtual void draw() {
            drawsetup();
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
            glDrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);
            glDisableVertexAttribArray(0);
        }
        virtual ~BasicModel() {}
};

class SingleColourModel : public BasicModel {
    protected:
        GLint colourloc = 0;
        GLfloat colour[4];
        void drawsetup() override{
            glUseProgram(shader);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
            glUniformMatrix4fv(matloc, 1, GL_FALSE, MVP);
            glUniform4fv(colourloc, 1, colour);
        }
    public:
        SingleColourModel(float* vertices_in, int16_t number_of_vertices, unsigned int* indices_in, uint16_t number_of_indices, Shader* modelshader, glm::mat4* mvp, float r, float g, float b, float a) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp) {
            colourloc = glGetUniformLocation(shader, "u_colour");
            colour[0] = r;
            colour[1] = g;
            colour[2] = b;
            colour[3] = a;
        }
        ~SingleColourModel() override{}
};

class ManyTextureModel : public BasicModel {
    protected:
        GLint arrayloc = 0;
        GLint samplers[8] = {};
        GLuint textures[8] = {};
        unsigned int texnum;
        virtual void texturesetup(){
            for (uint8_t i = 0; i < texnum; i++) {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, textures[i]);
            }
            glUniform1iv(arrayloc, texnum, samplers);
        }
    public:
        ManyTextureModel(float* vertices_in, int16_t number_of_vertices, unsigned int* indices_in, uint16_t number_of_indices, Texture** texturearray, uint8_t number_of_textures, Shader* modelshader, glm::mat4* mvp) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp) {
            arrayloc = glGetUniformLocation(shader, "u_texarray");
            texnum = number_of_textures;
            for (uint8_t i = 0; i < texnum; i++) {
                textures[i] = texturearray[i]->gettexture();
                samplers[i] = i;
            }
        }
        void draw() override {
            drawsetup();
            texturesetup();
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
            glDrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
        }
        ~ManyTextureModel() override {}
};

class SingleTextureModel : public BasicModel {
    protected:
        GLint texloc;
        GLuint image;
        GLint arrayloc = 0;
        unsigned int texnum;
        virtual void texturesetup(){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, image);
            glUniform1i(texloc, 0);
        }
    public:
        SingleTextureModel(float* vertices_in, int16_t number_of_vertices, unsigned int* indices_in, uint16_t number_of_indices, Texture* picture, Shader* modelshader, glm::mat4* mvp) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp){
            image = picture->gettexture();    
        }
        void draw() override {
            drawsetup();
            texturesetup();
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glDrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
        }
        ~SingleTextureModel() override {}
};

class DispModel : public BasicModel {
    protected:
        GLint arrayloc = 0;
        GLint samplers[8] = {};
        GLuint textures[8] = {};
        unsigned int texnum;
        unsigned int vertnum;
        float scroll_x1;
        float scroll_y1;
        float scroll_x2;
        float scroll_y2;
        virtual void texturesetup(){
            for (uint8_t i = 0; i < texnum; i++) {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, textures[i]);
            }
            glUniform1iv(arrayloc, texnum, samplers);
        }
        virtual void scroll(){
            for (unsigned int i = 0; i < vertnum; i++) {
                vertices[(9 * i) + 3] += scroll_x1;
                vertices[(9 * i) + 4] += scroll_y1;
                vertices[(9 * i) + 5] += scroll_x2;
                vertices[(9 * i) + 6] += scroll_y2;
                //Possibility of float overflow
                //will only happen after a long time
                //should quickly snap back to normal.
            }
        }
        virtual void regeneratebuffers(){
            glGenBuffers(1, &vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, vertnum * sizeof(float) * 9, vertices, GL_STATIC_DRAW);
        }
    public:
        DispModel(float* vertices_in, int16_t number_of_vertices, unsigned int* indices_in, uint16_t number_of_indices, Texture** texturearray, uint8_t number_of_textures, Shader* modelshader, glm::mat4* mvp, float x1, float y1, float x2, float y2) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp) {
            arrayloc = glGetUniformLocation(shader, "u_texarray");
            texnum = number_of_textures;
            for (uint8_t i = 0; i < texnum; i++) {
                textures[i] = texturearray[i]->gettexture();
                samplers[i] = i;
            }
            scroll_x1 = x1;
            scroll_y1 = y1;
            scroll_x2 = x2;
            scroll_y2 = y2;
            vertnum = number_of_vertices / 9;
        }
        void draw() override {
            drawsetup();
            texturesetup();
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
            glDrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            scroll();
            regeneratebuffers();
        }
        ~DispModel() override {}
};

class SubunitModel : public BasicModel {
    protected:
        GLint texloc;
        GLuint image;
        virtual void texturesetup(){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, image);
            glUniform1i(texloc, 0);
        }
    public:
        SubunitModel(float* vertices_in, int16_t number_of_vertices, unsigned int* indices_in, uint16_t number_of_indices, Texture* picture, Shader* modelshader, glm::mat4* mvp) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp) {
            texloc = glGetUniformLocation(shader, "u_texture");
            image = picture->gettexture();
        }
        void draw() override {
            drawsetup();
            texturesetup();
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(5 * sizeof(float)));
            glDrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
        }
        ~SubunitModel() override {}
};
