#include "Texture.hpp"
#include <glpp-extended/src/TextureLayout.h>
#include <img/src/Size.h>

Texture::Texture(std::string imagePath)
    :id{},
    image(img::Size(),0,nullptr) {
    image = p6::load_image_buffer(imagePath);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(image.width()), static_cast<GLsizei>(image.height()), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, id);
}
void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}