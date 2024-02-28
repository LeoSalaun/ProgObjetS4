#include "Boid.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"
#include <iostream>
#include <time.h>
#include <vector>

double rand01() {
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto distrib = std::uniform_real_distribution<double>{0.0, 1.0};

    return distrib(gen);
}

Boid::Boid() {
    color = glm::vec3(rand01() , rand01() , rand01());
    direction = vec((rand01()-0.5)/10,(rand01()-0.5)/10);
    position = vec((rand01()-0.5),(rand01()-0.5));
}

void Boid::display(p6::Context &ctx) {
    ctx.circle(
        p6::Center{position[0],position[1]},
        p6::Radius{0.2f}
    );
}

void Boid::updatePosition() {
        position += vec((rand01()-0.5)/10);
}

vec Boid::getPosition() const {
    return position;
}

glm::vec3 Boid::getColor() const {
    return color;
}

vec Boid::getDirection() const {
    return direction;
}