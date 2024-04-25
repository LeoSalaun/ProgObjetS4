#pragma once

#include <string>
#include "p6/p6.h"
#include <img/src/Image.h>

class Texture {
public:
    explicit Texture(std::string imagePath);

    void    bind();
    void    unbind();

private:
    GLuint id;
    img::Image image;
};