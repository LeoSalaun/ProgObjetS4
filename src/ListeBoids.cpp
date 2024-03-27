#include "ListeBoids.hpp"
#include <vector>
#include "Boid.hpp"

void ListeBoids::addBoid(Boid b)
{
    listeBoids.push_back(b);
}

void ListeBoids::update(float separation, float cohesion, float alignment)
{
    for (Boid& b : listeBoids)
    {
        b.applySteeringForces(listeBoids, separation, cohesion, alignment);
        b.updatePosition();
    }
}

void ListeBoids::display(p6::Context& ctx) const
{
    for (const Boid& b : listeBoids)
    {
        b.display(ctx);
    }
}
