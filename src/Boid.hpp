#pragma once
#include <vector>
#include "Model3D.hpp"
#include "glm/fwd.hpp"

using vec = glm::vec3;

class Boid {
public:
    Boid(int sizeBoid);

    void      display(glm::mat4 &ModelMatrix, glm::mat4 &ViewMatrix, glm::mat4 &ProjMatrix,
                      GLint uMVPMatrix, GLint uMVMatrix, GLint uNormalMatrix, Model3D &model) const;
    void      updatePosition();
    vec       getPosition() const;
    glm::vec3 getColor() const;
    vec       getDirection() const;
    vec       calculateSeparationForce(const std::vector<Boid>& boids, float separation);
    vec       calculateCohesionForce(const std::vector<Boid>& boids, float cohesion);
    vec       calculateAlignmentForce(const std::vector<Boid>& boids, float alignment);
    void      applySteeringForces(const std::vector<Boid>& boids, float separation, float cohesion, float alignment);

private:
    vec       position;
    glm::vec3 color;
    vec       direction;
    int       size;
};