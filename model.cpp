#include "model.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stb/stb_image.h>
#include <inttypes.h>

Shader::Shader(const char* VertexShader, const char* FragmentShader) {
    unsigned int vertexshader = compile(parse(VertexShader), GL_VERTEX_SHADER, VertexShader);
    unsigned int fragmentshader = compile(parse(FragmentShader), GL_FRAGMENT_SHADER, FragmentShader);
    glAttachShader(program, vertexshader);
    glAttachShader(program, fragmentshader);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
}
void Shader::bind() {
    glUseProgram(program);
}
void Shader::unbind() {
    glUseProgram(0);
}
GLuint Shader::getShader(){
    return program;
}
GLint Shader::getUniformLocation(const char* uniformname) {
    GLint location = glGetUniformLocation(program, uniformname);
    if (location == -1) {
        std::printf("WARNING: Uniform \"%s\" does not exist!\n", (char*)uniformname);
    }
    return location;
}
void Shader::setUniform4f(const char* uniformname, float v0, float v1, float v2, float v3) {
    glUniform4f(getUniformLocation(uniformname), v0, v1, v2, v3);
}
void Shader::setUniform1i(const char* uniformname, int i0) {
    glUniform1i(getUniformLocation(uniformname), i0);
}
void Shader::setUniformMat4f(const char* uniformname, glm::mat4& matrix) {
    glUniformMatrix4fv(getUniformLocation(uniformname), 1, GL_FALSE, &matrix[0][0]);
}
unsigned int Shader::compile(const char* source, unsigned int type, const char* shadername) {
    if(source != nullptr) {
        unsigned int id = glCreateShader(type);
        glShaderSource(id, 1, &source, nullptr);
        glCompileShader(id);
        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            int len;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
            char* message = (char*)malloc(len * sizeof(char));
            glGetShaderInfoLog(id, len, &len, message);
            std::printf("%s failed to compile!\n", shadername);
            std::printf("%s\n", message);
            glDeleteShader(id);
            return 0;
        }
        return id;
    }
    else {
        std::printf("Shader %s failed to generate.\n", shadername);
        return -1;
    }
}
const char* Shader::parse(const char* filepath) {
    FILE* fp;
    fp = fopen(filepath, "rb");
    if (fp != nullptr) {
        fseek(fp, 0, SEEK_END);
        int length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        char* shadercode = (char*)malloc((length + 1) * sizeof(char));
        if (shadercode) {
            shadercode[length] = '\0';
            fread(shadercode, 1, length, fp);
            fclose(fp);
            return(shadercode);
        }
        else {
            std::printf("const char* array shadercode could not be allocated memory, and was a nullptr pointer.\n");
            return(nullptr);
        }
    }
    else {
        std::printf("Shader file %s could not be found/opened.", filepath);
        return nullptr;
    }
}
Shader::Shader::~Shader() {
    glDeleteProgram(program);
}

Texture::Texture(const char* imagefile, GLint texture_mag_filter, GLint texture_min_filter, GLint texture_wrap_s, GLint texture_wrap_t, float* border_colour) {
    int width, height, bpp;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* localbuffer = stbi_load(imagefile, &width, &height, &bpp, 4);
    if (!localbuffer) {
        std::printf("Image file %s could not be found or opened.\n", imagefile);
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrap_t);
    if ((texture_wrap_s == GL_CLAMP_TO_EDGE || texture_wrap_t == GL_CLAMP_TO_EDGE) && border_colour != nullptr) {
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_colour);
    }
    else if (texture_wrap_s == GL_CLAMP_TO_EDGE || texture_wrap_t == GL_CLAMP_TO_EDGE) {
        float border[4] = { 0,0,0,0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localbuffer);
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (localbuffer) {
        stbi_image_free(localbuffer);
    }
}
GLuint Texture::getTexture(){
    return texture;
}
void Texture::bindTexture(unsigned int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);
}
void Texture::unbindTexture(void) {
    glBindTexture(GL_TEXTURE_2D, 0);
}
Texture::~Texture() {
    glDeleteTextures(1, &texture);
}

BasicModel::BasicModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Shader* modelshader, glm::mat4* mvp) {
    floatnum = number_of_vertices;
    indexnum = number_of_indices;
    stride = 3;
    vertexcount = (GLuint)(floatnum / stride);
    shader = modelshader->getShader();
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
void BasicModel::regenerateVertexBuffer(){
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, floatnum * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
}
void BasicModel::drawSetup(){
    glUseProgram(shader);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
    glUniformMatrix4fv(matloc, 1, GL_FALSE, MVP);
} 
glm::vec3 BasicModel::getMax(){
    return glm::vec3(stats[0][0],stats[1][0],stats[2][0]);
}
glm::vec3 BasicModel::getMin(){
    return glm::vec3(stats[0][1],stats[1][1],stats[2][1]);
}
glm::vec3 BasicModel::getMiddle(){
    return glm::vec3(stats[0][2],stats[1][2],stats[2][2]);
}
glm::vec3 BasicModel::getSize(){
    return glm::vec3(stats[0][3],stats[1][3],stats[2][3]);
}
void BasicModel::translate(GLfloat x, GLfloat y, GLfloat z){
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
    regenerateVertexBuffer();
}
void BasicModel::scaleCentre(GLfloat scalar){
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
    regenerateVertexBuffer();
}
void BasicModel::scaleBase(GLfloat scalar){
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
    regenerateVertexBuffer();
} 
void BasicModel::draw() {
    drawSetup();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
    glDrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);
    glDisableVertexAttribArray(0);
}
BasicModel::~BasicModel() {}

SingleColourModel::SingleColourModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Shader* modelshader, glm::mat4* mvp, float r, float g, float b, float a) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp) {
    colourloc = glGetUniformLocation(shader, "u_colour");
    colour[0] = r;
    colour[1] = g;
    colour[2] = b;
    colour[3] = a;
}
void SingleColourModel::drawSetup(){
    glUseProgram(shader);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
    glUniformMatrix4fv(matloc, 1, GL_FALSE, MVP);
    glUniform4fv(colourloc, 1, colour);
}
SingleColourModel::~SingleColourModel(){}

ManyTextureModel::ManyTextureModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Texture** texturearray, uint8_t number_of_textures, Shader* modelshader, glm::mat4* mvp) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp) {
            stride = 6;
            arrayloc = glGetUniformLocation(shader, "u_texarray");
            texnum = number_of_textures;
            for (uint8_t i = 0; i < texnum; i++) {
                textures[i] = texturearray[i]->getTexture();
                samplers[i] = i;
            }
        }
void ManyTextureModel::textureSetup(){
    for (uint8_t i = 0; i < texnum; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    glUniform1iv(arrayloc, texnum, samplers);
}
void ManyTextureModel::draw(){
    drawSetup();
    textureSetup();
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
ManyTextureModel::~ManyTextureModel(){}

SingleTextureModel::SingleTextureModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Texture* picture, Shader* modelshader, glm::mat4* mvp) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp){
    stride = 5;
    if(picture != nullptr){
        image = picture->getTexture(); 
    }
}
void SingleTextureModel::textureSetup(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, image);
    glUniform1i(texloc, 0);
}
void SingleTextureModel::draw(){
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
SingleTextureModel::~SingleTextureModel(){}

DispModel::DispModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Texture** texturearray, uint8_t number_of_textures, Shader* modelshader, glm::mat4* mvp, float x1, float y1, float x2, float y2) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp) {
    stride = 9;
    arrayloc = glGetUniformLocation(shader, "u_texarray");
    texnum = number_of_textures;
    for (uint8_t i = 0; i < texnum; i++) {
        textures[i] = texturearray[i]->getTexture();
        samplers[i] = i;
    }
    scroll_x1 = x1;
    scroll_y1 = y1;
    scroll_x2 = x2;
    scroll_y2 = y2;
}
void DispModel::textureSetup(){
    for (uint8_t i = 0; i < texnum; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    glUniform1iv(arrayloc, texnum, samplers);
}
void DispModel::scroll(){
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
void DispModel::draw(){
    drawSetup();
    textureSetup();
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
    regenerateVertexBuffer();
}
DispModel::~DispModel(){}

SubunitModel::SubunitModel(GLfloat* vertices_in, GLuint number_of_vertices, GLuint* indices_in, GLuint number_of_indices, Texture* picture, Shader* modelshader, glm::mat4* mvp) : BasicModel(vertices_in, number_of_vertices, indices_in, number_of_indices, modelshader, mvp) {
    stride = 9;
    texloc = glGetUniformLocation(shader, "u_texture");
    image = picture->getTexture();
}
void SubunitModel::textureSetup(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, image);
    glUniform1i(texloc, 0);
}
void SubunitModel::draw(){
    drawSetup();
    textureSetup();
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
SubunitModel::~SubunitModel(){}
