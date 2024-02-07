#pragma once
#include <string>
#include <vector>
#include "glm/fwd.hpp"
#include "p6/p6.h"

using vec = glm::vec2;

class Boid {
public:
    Boid();

    void      display(p6::Context& ctx);
    void      updatePosition();
    vec       getPosition() const;
    glm::vec3 getColor() const;
    vec       getDirection() const;
    //    const static int DIMENSION = 2;

private:
    vec       position;
    glm::vec3 color;
    vec       direction;
};