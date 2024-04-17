#pragma once

#include "p6/p6.h"
#include "glm/glm.hpp"
#include "glimac/default_shader.hpp"

class VBO {

    private:
        GLuint id;
    
    public:
        VBO();
        ~VBO();
        VBO(const VBO & other) = delete;
        VBO& operator=(const VBO & other) = delete;
        VBO(VBO && other) noexcept;
        VBO& operator=(VBO && other) noexcept;

        GLuint getId() const;
        void setId(const GLuint &newId);

        void bind(GLenum target);
        void unbind(GLenum target);
};