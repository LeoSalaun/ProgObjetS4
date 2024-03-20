#include "Boid.hpp"
#include <iostream>
#include <vector>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

double rand01()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<double>{0.0, 1.0};

    return distrib(gen);
}

Boid::Boid()
 : position{vec((rand01() - 0.5f) * 0.8f, (rand01() - 0.5f) * 0.8f)},
   direction{vec((rand01() - 0.5f) * 0.001f, (rand01() - 0.5f) * 0.001f)},
   color{ glm::vec3(rand01(), rand01(), rand01())} {}

Boid::Boid(float wander)
 : position{vec((rand01() - 0.5) * 0.8, (rand01() - 0.5) * 0.8)},
   direction{vec((rand01() - 0.5) * wander, (rand01() - 0.5) * wander)},
   color{ glm::vec3(rand01(), rand01(), rand01())} {}

void Boid::display(p6::Context& ctx) const
{
    ctx.circle(
        p6::Center{position[0], position[1]},
        p6::Radius{0.02f}
    );
}

void Boid::updatePosition()
{
    /*float sumDirection = 0.f;
    for (size_t i=0 ; i<direction.length() ; i++) {
        sumDirection += direction[i];
    }
    direction -= vec(sumDirection/direction.length());*/
    //direction *= 0.001f;
    // for (size_t i=0 ; i<direction.length() ; i++) {
    //     if (direction[i] > 0.01f) {
    //         direction /= (2*direction[i]);
    //     }
    // }
    //direction = vec(0.001f);

    // std::cout << direction[0] << " " << direction[1] << std::endl;

    //std::cout << (rand01() - 0.5f) * 0.8f << std::endl;

    position += direction;
    if (position.x > 0.5f)
        position.x -= 1.f;
    if (position.x < -0.5f)
        position.x += 1.f;
    if (position.y > 0.5f)
        position.y -= 1.f;
    if (position.y < -0.5f)
        position.y += 1.f;
}

vec Boid::getPosition() const
{
    return position;
}

glm::vec3 Boid::getColor() const
{
    return color;
}

vec Boid::getDirection() const
{
    return direction;
}

void Boid::setWanderStrength(float wander) {
    direction *= wander;
}

void Boid::calculateSeparationForce(const std::vector<Boid>& listeBoids, float separation)
{
    vec totalForce = vec(0.f);

    for (const Boid& otherBoid : listeBoids)
    {
        if (&otherBoid == this)
            continue;
        float distance            = glm::distance(position, otherBoid.getPosition());
        vec   separationDirection = position - otherBoid.getPosition();
        //std::cout << "position : " << position[0] << " " << position[1] << std::endl;
    
        if (distance > 0.f && distance < 2.f) {
            totalForce += separationDirection / (distance);
        }
    }
    direction += glm::normalize(totalForce) * separation;
    // std::cout << totalForce[0] << " , " << totalForce[1] << std::endl;
    //std::cout << "separation : " << direction[0] << " " << direction[1] << std::endl;
}

void Boid::calculateCohesionForce(const std::vector<Boid>& boids, float cohesion)
{
    vec averagePosition = vec(0, 0);
    for (const Boid& b : boids)
    {
        averagePosition += b.getPosition();
    }
    averagePosition /= boids.size();
    direction += glm::normalize(averagePosition) * cohesion;
    //std::cout << cohesion << std::endl;
}

void Boid::calculateAlignmentForce(const std::vector<Boid>& boids, float alignment)
{
    vec averageDirection(0.0f);

    for (const Boid& otherBoid : boids)
    {
        if (&otherBoid != this)
        {
            averageDirection += otherBoid.getDirection();
        }  
    }
     if (!boids.empty())
        {
            averageDirection /= static_cast<float>(boids.size());
        }
        if (glm::length(averageDirection) > 0)
        {
            direction += glm::normalize(averageDirection) * alignment;
        }
    direction += vec{0.f};
}

void Boid::applySteeringForces(const std::vector<Boid>& boids, float separation, float cohesion, float alignment) {
    calculateSeparationForce(boids, separation);
    calculateCohesionForce(boids, cohesion);
    calculateAlignmentForce(boids, alignment);
    direction = glm::normalize(direction);
    direction *= 0.001;
    std::cout << direction[0] << std::endl;
};
