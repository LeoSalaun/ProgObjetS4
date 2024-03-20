#pragma once
#include <vector>
#include "glm/fwd.hpp"
#include "p6/p6.h"

using vec = glm::vec2;

class Boid {
public:
    Boid();
    Boid(float wander);

    void      display(p6::Context& ctx) const;
    void      updatePosition();
    vec       getPosition() const;
    glm::vec3 getColor() const;
    vec       getDirection() const;
    void      setWanderStrength(float wander);
    //    const static int DIMENSION = 2;
    void calculateSeparationForce(const std::vector<Boid>& boids, float separation);
    void  calculateCohesionForce(const std::vector<Boid>& boids, float cohesion);
    void  calculateAlignmentForce(const std::vector<Boid>& boids, float alignment);
    void applySteeringForces(const std::vector<Boid>& boids, float separation, float cohesion, float alignment);

private:
    vec       position;
    glm::vec3 color;
    vec       direction;
};