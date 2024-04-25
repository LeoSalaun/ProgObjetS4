#include "Model3D.hpp"

Model3D::Model3D(std::vector<glimac::ShapeVertex> verticesList) :vertices(verticesList) {
    enableVertex(vbo, vao, vertices);
}

Model3D::Model3D(Object3D object3D) :vertices(object3D.vertices) {
    enableVertex(vbo, vao, vertices);
}

void Model3D::drawObject(glm::mat4 &ViewMatrix, glm::mat4 &ModelMatrix, glm::mat4 &ProjMatrix, GLint uMVPMatrix, GLint uMVMatrix, GLint uNormalMatrix) {
    vao.bind();

    glm::mat4 MVMatrix = ViewMatrix*ModelMatrix;
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    glUniformMatrix4fv(uMVPMatrix, 1.f, GL_FALSE, glm::value_ptr(ProjMatrix*MVMatrix));
    glUniformMatrix4fv(uMVMatrix, 1.f, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrix, 1.f, GL_FALSE, glm::value_ptr(NormalMatrix));
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(vertices.size()));

    vao.unbind();
}