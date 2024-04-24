#include "FreeflyCamera.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"

FreeflyCamera::FreeflyCamera()
: m_Position(0.f), m_Phi(M_PI), m_Theta(0.f) {
    computeDirectionVectors();
}

void FreeflyCamera::computeDirectionVectors() {
    m_FrontVector = glm::vec3(std::cos(m_Theta)*std::sin(m_Phi), std::sin(m_Theta), std::cos(m_Theta)*std::cos(m_Phi));
    m_LeftVector  = glm::vec3(std::sin(m_Phi+M_PI/2.f),          0.f,               std::cos(m_Phi+M_PI/2.f));
    m_UpVector = glm::cross(m_FrontVector,m_LeftVector);
}

void FreeflyCamera::moveFront(float t) {
    m_Position -= t * m_FrontVector;
    replaceCamera();
    computeDirectionVectors();
}

void FreeflyCamera::moveLeft(float t) {
    m_Position -= t * m_LeftVector;
    replaceCamera();
    computeDirectionVectors();
}

void FreeflyCamera::rotateLeft(float degrees) {
    m_Phi += degrees*static_cast<float>(M_PI)/180.f;
    computeDirectionVectors();
}

void FreeflyCamera::rotateUp(float degrees) {
    m_Theta += degrees*static_cast<float>(M_PI)/180.f;
    computeDirectionVectors();
}

glm::mat4 FreeflyCamera::getViewMatrix() const {
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}

void FreeflyCamera::replaceCamera() {
    if (m_Position.x < -10.f) {
        m_Position.x = -9.9f;
    }
    if (m_Position.x > 10.f) {
        m_Position.x = 9.9f;
    }

    if (m_Position.y < -10.f) {
        m_Position.y = -9.9f;
    }
    if (m_Position.y > 10.f) {
        m_Position.y = 9.9f;
    }

    if (m_Position.z < -10.f) {
        m_Position.z = -9.9f;
    }
    if (m_Position.z > 10.f) {
        m_Position.z = 9.9f;
    }
}