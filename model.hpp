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
        void regeneratevertexbuffer(){
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, floatnum * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
        }
        virtual void drawSetup(){
            glUseProgram(shader);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
            glUniformMatrix4fv(matloc, 1, GL_FALSE, MVP);
        }
    public:
        BasicModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Shader* modelshader, glm::mat4* mvp) {
            floatnum = number_of_vertices;
            indexnum = number_of_indices;
            stride = 3;
            vertexcount = (GLuint)(floatnum / stride);
            shader = modelshader->getshader();
            matloc = glGetUniformLocation(shader, "MVP");
            MVP = (GLfloat*)mvp;
            vertices = vertices_in;
            indices = indices_in;
            indexnum = number_of_indices;
            for(GLuint i = 0; i < vertexcount; i++){
                for(GLuint j = 0; j < 3; j++){
                    if( stats[j][0] < vertices[stride * i + j]){
                        stats[j][0] = vertices[stride * i + j];
                    }
                    if( stats[j][1] > vertices[stride * i + j]){
                        stats[j][1] = vertices[stride * i + j];
                    }
                }
            }
            for(GLuint i = 0; i < 2; i++){
                stats[i][2] = (stats[i][0] + stats[i][1]) / 2;
                stats[i][3] = stats[i][0] - stats[i][1];
            }
            glGenBuffers(1, &vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, floatnum * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glGenBuffers(1, &indexbuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexnum * sizeof(GLfloat), indices, GL_DYNAMIC_DRAW);
        } 
        glm::vec3 getMax(){
            return glm::vec3(stats[0][0],stats[1][0],stats[2][0]);
        }
        glm::vec3 getMin(){
            return glm::vec3(stats[0][1],stats[1][1],stats[2][1]);
        }
        glm::vec3 getMiddle(){
            return glm::vec3(stats[0][2],stats[1][2],stats[2][2]);
        }
        glm::vec3 getSize(){
            return glm::vec3(stats[0][3],stats[1][3],stats[2][3]);
        }
        void translate(GLfloat x, GLfloat y, GLfloat z){
            for(GLuint i = 0; i < vertexcount; i++){
                vertices[stride * i + 0] += x;
                vertices[stride * i + 1] += y;
                vertices[stride * i + 2] += z;
            }
            for(GLuint i = 0; i < 2; i++){
                stats[0][i] += x;
                stats[1][i] += y;
                stats[2][i] += z;
            }
            regeneratevertexbuffer();
        }
        void scaleCentre(GLfloat scalar){
            for(GLuint i = 0; i < vertexcount; i++){
                vertices[stride * i + 0] = ((vertices[stride * i + 0] - stats[0][2]) * scalar) + stats[0][2];
                vertices[stride * i + 1] = ((vertices[stride * i + 1] - stats[1][2]) * scalar) + stats[1][2];
                vertices[stride * i + 2] = ((vertices[stride * i + 2] - stats[2][2]) * scalar) + stats[2][2];
            }
            for(GLuint i = 0; i < 2; i++){
                stats[i][3] = stats[i][3] * scalar;
                stats[i][0] = stats[i][2] + stats[i][3] / 2;
                stats[i][1] = stats[i][2] - stats[i][3] / 2;
            }
            regeneratevertexbuffer();
        }
        void scaleBase(GLfloat scalar){
            for(GLuint i = 0; i < vertexcount; i++){
                vertices[stride * i + 0] = ((vertices[stride * i + 0] - stats[0][2]) * scalar) + stats[0][2];
                vertices[stride * i + 1] = ((vertices[stride * i + 1] - stats[1][1]) * scalar) + stats[1][1];
                vertices[stride * i + 2] = ((vertices[stride * i + 2] - stats[2][2]) * scalar) + stats[2][2];
            }
            for(GLuint i = 0; i < 3; i++){
                stats[i][3] = stats[i][3] * scalar;
                stats[i][0] = stats[i][2] + stats[i][3] / 2;
                stats[i][1] = stats[i][2] - stats[i][3] / 2;
            }
            stats[1][1] += stats[2][3] / 2;
            regeneratevertexbuffer();
        } 
        virtual void draw() {
            drawSetup();
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
            glDrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);
            glDisableVertexAttribArray(0);
        }
        virtual ~BasicModel() {}
};

class SingleColourModel : public BasicModel {
    protected:
        GLint colourloc = 0;
        GLfloat colour[4];
        void drawSetup() override{
            glUseProgram(shader);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
            glUniformMatrix4fv(matloc, 1, GL_FALSE, MVP);
            glUniform4fv(colourloc, 1, colour);
        }
    public:
        SingleColourModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Shader* modelshader, glm::mat4* mvp, float r, float g, float b, float a) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp) {
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
        ManyTextureModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Texture** texturearray, uint8_t number_of_textures, Shader* modelshader, glm::mat4* mvp) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp) {
            stride = 6;
            arrayloc = glGetUniformLocation(shader, "u_texarray");
            texnum = number_of_textures;
            for (uint8_t i = 0; i < texnum; i++) {
                textures[i] = texturearray[i]->gettexture();
                samplers[i] = i;
            }
        }
        void draw() override {
            drawSetup();
            texturesetup();
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
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
        SingleTextureModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Texture* picture, Shader* modelshader, glm::mat4* mvp) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp){
            stride = 5;
            image = picture->gettexture();    
        }
        void draw() override {
            drawSetup();
            texturesetup();
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
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
            for (unsigned int i = 0; i < vertexcount; i++) {
                vertices[stride * i + 3] += scroll_x1;
                vertices[stride * i + 4] += scroll_y1;
                vertices[stride * i + 5] += scroll_x2;
                vertices[stride * i + 6] += scroll_y2;
                //Possibility of float overflow
                //will only happen after a long time
                //should quickly snap back to normal.
            }
        }
    public:
        DispModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Texture** texturearray, uint8_t number_of_textures, Shader* modelshader, glm::mat4* mvp, float x1, float y1, float x2, float y2) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp) {
            stride = 9;
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
        }
        void draw() override {
            drawSetup();
            texturesetup();
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));
            glDrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            scroll();
            regeneratevertexbuffer();
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
        SubunitModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Texture* picture, Shader* modelshader, glm::mat4* mvp) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp) {
            stride = 9;
            texloc = glGetUniformLocation(shader, "u_texture");
            image = picture->gettexture();
        }
        void draw() override {
            drawSetup();
            texturesetup();
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
            glDrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
        }
        ~SubunitModel() override {}
};
