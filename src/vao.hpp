#pragma once

#include "p6/p6.h"
#include "glm/glm.hpp"
#include "glimac/default_shader.hpp"

class VAO {

    private:
        GLuint id;
    
    public:
        VAO();
        ~VAO();
        VAO(const VAO & other) = delete;
        VAO& operator=(const VAO & other) = delete;
        VAO(VAO && other) noexcept;
        VAO& operator=(VAO && other) noexcept;

        GLuint getId() const;
        void setId(const GLuint &newId);

        void bind();
        void unbind();
};