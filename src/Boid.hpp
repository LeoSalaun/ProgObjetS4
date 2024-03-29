#pragma once
#include <vector>
#include "glm/fwd.hpp"
#include "p6/p6.h"

using vec = glm::vec2;

class Boid {
public:
    Boid();
    explicit Boid(float wander);

    void      display(p6::Context& ctx) const;
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
};