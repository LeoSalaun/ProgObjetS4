#include "Boid.hpp"
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "glm/fwd.hpp"
#include "p6/p6.h"

const float SEPARATION_STRENGTH = 1.f / 100000;
const float ALIGNMENT_STRENGTH  = 1 / 10;
const float WANDER_STRENGTH     = 1.f / 100000;

double rand01()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<double>{0.0, 1.0};

    return distrib(gen);
}

Boid::Boid()
{
    color     = glm::vec3(rand01(), rand01(), rand01());
    direction = vec((rand01() - 0.5) * WANDER_STRENGTH, (rand01() - 0.5) * WANDER_STRENGTH);
    position  = vec((rand01() - 0.5) * 0.8, (rand01() - 0.5) * 0.8);
}

void Boid::display(p6::Context& ctx)
{
    ctx.circle(
        p6::Center{position[0], position[1]},
        p6::Radius{0.02f}
    );
}

vec Boid::CalculateAlignmentForce(const std::vector<Boid>& boids)
{
    vec averageDirection(0.0f);

    for (const Boid& otherBoid : boids)
    {
        if (&otherBoid != this)
        {
            averageDirection += otherBoid.getDirection();
        }
        if (!boids.empty())
        {
            averageDirection /= static_cast<float>(boids.size());
        }
        if (glm::length(averageDirection) > 0)
        {
            return glm::normalize(averageDirection) * ALIGNMENT_STRENGTH;
        }
    }
}

void Boid::updatePosition(const std::vector<Boid>& boids, vec cohesionForce)
{
    vec alignmentForce = CalculateAlignmentForce(boids);
    direction += alignmentForce;

    position += direction;
    if (position.x > 0.5)
        position.x -= 1.f;
    if (position.x < -0.5)
        position.x += 1.f;
    if (position.y > 0.5)
        position.y -= 1.f;
    if (position.y < -0.5)
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

void Boid::CalculateSeparationForce(const std::vector<Boid>& listeBoids)
{
    vec totalForce = vec(0);

    for (const Boid& otherBoid : listeBoids)
    {
        if (&otherBoid == this)
            continue;
        float distance            = glm::distance(position, otherBoid.getPosition());
        vec   separationDirection = position - otherBoid.getPosition();

        if (distance > 0)
        {
            totalForce += separationDirection / (distance);
        }
    }
    direction += totalForce * SEPARATION_STRENGTH;
    // std::cout << totalForce[0] << " , " << totalForce[1] << std::endl;
}
