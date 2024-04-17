#include "vao.hpp"

VAO::VAO() {
    glGenVertexArrays(1, &id);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &id);
}

VAO::VAO(VAO && other) noexcept
    : id{other.getId()} {
    other.setId(0);
}

VAO& VAO::operator=(VAO && other) noexcept {
    if (this != &other) {         // Make sure that we don't do silly things when we try to move an object to itself
        glDeleteVertexArrays(1, &id); // Delete the previous object
        id = other.getId();      // Copy the object
        other.setId(0);            // Make sure that other won't delete the _id we just copied
    }
    return *this;
}

GLuint VAO::getId() const {
    return this->id;
}

void VAO::setId(const GLuint &newId) {
    this->id = newId;
}

void VAO::bind() {
    glBindVertexArray(id);
}

void VAO::unbind() {
    glBindVertexArray(0);
}