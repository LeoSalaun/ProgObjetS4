#pragma once

#include "p6/p6.h"
#include "glm/glm.hpp"
#include "glimac/default_shader.hpp"

class IBO {

    private:
        GLuint id;
    
    public:
        IBO();
        ~IBO();
        IBO(const IBO & other) = delete;
        IBO& operator=(const IBO & other) = delete;
        IBO(IBO && other) noexcept;
        IBO& operator=(IBO && other) noexcept;

        GLuint getId() const;
        void setId(const GLuint &newId);

        void bind();
        void unbind();
};