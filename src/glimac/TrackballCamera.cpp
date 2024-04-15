#include "TrackballCamera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

TrackballCamera::TrackballCamera()
: m_Distance(0.f), m_AngleX(0.f), m_AngleY(0.f) {}

void TrackballCamera::moveFront(float delta) {
    m_Distance += delta;
}

void TrackballCamera::rotateLeft(float degrees) {
    m_AngleX += degrees;
    if (m_AngleX < 0.f)   m_AngleX += 360.f;
    if (m_AngleX > 360.f) m_AngleX -= 360.f;
}

void TrackballCamera::rotateUp(float degrees) {
    m_AngleY += degrees;
    if (m_AngleY < 0.f)   m_AngleX += 360.f;
    if (m_AngleY > 360.f) m_AngleX -= 360.f;
}

glm::mat4 TrackballCamera::getViewMatrix() const {
    glm::mat4 ViewMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -m_Distance));
    ViewMatrix = glm::rotate(ViewMatrix, -m_AngleX*static_cast<float>(M_PI)/180.f, glm::vec3(1.f,0.f,0.f));
    ViewMatrix = glm::rotate(ViewMatrix, -m_AngleY*static_cast<float>(M_PI)/180.f, glm::vec3(0.f,1.f,0.f));
    return ViewMatrix;
}