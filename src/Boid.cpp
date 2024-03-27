#include "Boid.hpp"
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
   color{ glm::vec3(rand01(), rand01(), rand01())},
   direction{vec((rand01() - 0.5f) * 0.001f, (rand01() - 0.5f) * 0.001f)} {}

Boid::Boid(float wander)
 : position{vec((rand01() - 0.5) * 0.8, (rand01() - 0.5) * 0.8)},
   color{ glm::vec3(rand01(), rand01(), rand01())},
   direction{vec((rand01() - 0.5) * wander, (rand01() - 0.5) * wander)} {}

void Boid::display(p6::Context& ctx) const
{
    ctx.circle(
        p6::Center{position[0], position[1]},
        p6::Radius{0.02f}
    );
}

void Boid::updatePosition()
{
    position += direction;

    if (position.x >  0.5f)
        position.x -= 1.f;
    if (position.x < -0.5f)
        position.x += 1.f;
    if (position.y >  0.5f)
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

vec Boid::calculateSeparationForce(const std::vector<Boid>& listeBoids, float separation)
{
    vec totalForce = vec(0.f);

    for (const Boid& otherBoid : listeBoids)
    {
        if (&otherBoid == this)
            continue;
        float distance            = glm::distance(position, otherBoid.getPosition());
        vec   separationDirection = position - otherBoid.getPosition();
        if (distance > 0.f && distance < .05f) {
            totalForce += separationDirection / (distance);
        }
    }
    return totalForce * separation;
}

vec Boid::calculateCohesionForce(const std::vector<Boid>& boids, float cohesion)
{
    vec averagePosition = vec(0, 0);
    for (const Boid& otherBoid : boids)
    {
        float distance = glm::distance(position, otherBoid.getPosition());
        if (distance > 0 && distance < .002f)
            averagePosition += otherBoid.getPosition();
    }
    averagePosition /= boids.size();
    return (averagePosition-position) * cohesion;
}

vec Boid::calculateAlignmentForce(const std::vector<Boid>& boids, float alignment)
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
    if (glm::length(averageDirection) > 0) {
        return averageDirection * alignment;
    }
    return vec{0.f};
}

void Boid::applySteeringForces(const std::vector<Boid>& boids, float separation, float cohesion, float alignment) {
    vec totalForce = calculateSeparationForce(boids, separation)
                   + calculateCohesionForce(boids, cohesion)
                   + calculateAlignmentForce(boids, alignment);
    direction += glm::normalize(totalForce);
    direction *= 0.001f;
};
