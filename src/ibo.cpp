#include "ibo.hpp"

IBO::IBO() {
    glGenBuffers(1, &id);
}

IBO::~IBO() {
    glDeleteBuffers(1, &id);
}

IBO::IBO(IBO && other) noexcept
    : id{other.getId()} {
    other.setId(0);
}

IBO& IBO::operator=(IBO && other) noexcept {
    if (this != &other) {         // Make sure that we don't do silly things when we try to move an object to itself
        glDeleteBuffers(1, &id); // Delete the previous object
        id = other.getId();      // Copy the object
        other.setId(0);            // Make sure that other won't delete the _id we just copied
    }
    return *this;
}

GLuint IBO::getId() const {
    return this->id;
}

void IBO::setId(const GLuint &newId) {
    this->id = newId;
}

void IBO::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id);
}

void IBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}