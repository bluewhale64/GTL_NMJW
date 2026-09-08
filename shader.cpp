#include "shader.hpp"

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
        printf("WARNING: Uniform \"%s\" does not exist!\n", (char*)uniformname);
    }
    return location;
}
void Shader::setUniform4f(const char* uniformname, float v0, float v1, float v2, float v3) {
    glUniform4f(getUniformLocation(uniformname), v0, v1, v2, v3);
}
void Shader::setUniform1i(const char* uniformname, int i0) {
    glUniform1i(getUniformLocation(uniformname), i0);
}
void Shader::setUniformMat4f(const char* uniformname, mat4& matrix) {
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
            printf("%s failed to compile!\n", shadername);
            printf("%s\n", message);
            glDeleteShader(id);
            return 0;
        }
        return id;
    }
    else {
        printf("Shader %s failed to generate.\n", shadername);
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
            printf("const char* array shadercode could not be allocated memory, and was a nullptr pointer.\n");
            return(nullptr);
        }
    }
    else {
        printf("Shader file %s could not be found/opened.", filepath);
        return nullptr;
    }
}
Shader::Shader::~Shader() {
    glDeleteProgram(program);
}