#include "ListeBoids.hpp"
#include <iostream>
#include <ostream>
#include <vector>
#include "Boid.hpp"

static constexpr float COHESION_STRENGTH { 1.f/ 1000000000.f};


void ListeBoids::newBoid()
{
    Boid b;
    listeBoids.push_back(b);
}

void ListeBoids::addBoid(Boid b)
{
    listeBoids.push_back(b);
}

void ListeBoids::update()
{
    for (Boid& b : listeBoids)
    {
        b.CalculateSeparationForce(listeBoids);
        vec cohesionForce = calculateCohesionForce();
        b.updatePosition(listeBoids);
    }
}

void ListeBoids::display(p6::Context& ctx) const
{
    for (const Boid& b : listeBoids)
    {
        b.display(ctx);
    }
}

vec ListeBoids::calculateCohesionForce()
{
    averagePosition = vec(0, 0);
    for (const Boid& b : listeBoids)
    {
        averagePosition += b.getPosition();
    }
    averagePosition /= listeBoids.size();
    return averagePosition * COHESION_STRENGTH;
}
