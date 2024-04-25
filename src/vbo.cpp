#include "vbo.hpp"

VBO::VBO() {
    glGenBuffers(1, &id);
}

VBO::~VBO() {
    glDeleteBuffers(1, &id);
}

// VBO::VBO(VBO && other) noexcept
//     : id{other.getId()} {
//     other.setId(0);
// }

// VBO& VBO::operator=(VBO && other) noexcept {
//     if (this != &other) {         // Make sure that we don't do silly things when we try to move an object to itself
//         glDeleteBuffers(1, &id); // Delete the previous object
//         id = other.getId();      // Copy the object
//         other.setId(0);            // Make sure that other won't delete the _id we just copied
//     }
//     return *this;
// }

GLuint VBO::getId() const {
    return this->id;
}

void VBO::setId(const GLuint &newId) {
    this->id = newId;
}

void VBO::bind(GLenum target) {
    glBindBuffer(target,id);
}

void VBO::unbind(GLenum target) {
    glBindBuffer(target,0);
}