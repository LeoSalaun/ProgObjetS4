#pragma once

#include "functions.hpp"
#include "glimac/common.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <img/src/Image.h>

class Model3D {
    private:
        std::vector<glimac::ShapeVertex> vertices;
        VBO vbo;
        VAO vao;

    public:
        explicit Model3D(std::vector<glimac::ShapeVertex> verticesList);
        explicit Model3D(Object3D object3D);

        void drawObject(glm::mat4 &ViewMatrix, glm::mat4 &ModelMatrix, glm::mat4 &ProjMatrix, GLint uMVPMatrix, GLint uMVMatrix, GLint uNormalMatrix);
};