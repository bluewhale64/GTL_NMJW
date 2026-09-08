#include "texture.hpp"
#include <stb/stb_image.h>

Texture::Texture(const char* imagefile, GLint texture_mag_filter, GLint texture_min_filter, GLint texture_wrap_s, GLint texture_wrap_t, float* border_colour) {
    int width, height, bpp;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* localbuffer = stbi_load(imagefile, &width, &height, &bpp, 4);
    if (!localbuffer) {
        printf("Image file %s could not be found or opened.\n", imagefile);
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
void Texture::unbindTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
Texture::~Texture() {
    glDeleteTextures(1, &texture);
}