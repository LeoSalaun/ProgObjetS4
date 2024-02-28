#include "Boid.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"
#include <iostream>
#include <time.h>

Boid::Boid()
: position(), direction() {
    srand(time(NULL));
    color = glm::vec3(static_cast<float>(rand()%256)/256 , static_cast<float>(rand()%256)/256 , static_cast<float>(rand()%256)/256);
    direction = vec(static_cast<float>(rand()%100-50)/10000);
}

void Boid::display(p6::Context &ctx) {
    ctx.circle(
        p6::Center{position[0],position[1]},
        p6::Radius{0.2f}
    );
}

void Boid::updatePosition() {
        direction = vec(static_cast<float>(rand()%100-50)/1000);
        position += direction;
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

void Boid::CalculateSeparationForce(const std::vector<Boid>& boids){

    float totalForce =  0;
    float separationDistance = 1.0f;

    for(const auto& otherBoid : boids){
        if (&otherBoid != this) { 
            float distance = glm::distance(position, otherBoid.getPosition());
            vec separationDirection = position - otherBoid.getPosition();
        
            if (distance > 0) {
                totalForce += glm::length(separationDirection) / distance;
            }
        }
    }
}