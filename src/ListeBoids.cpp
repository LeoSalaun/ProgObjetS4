#include "ListeBoids.hpp"
#include <vector>
#include "Boid.hpp"

void ListeBoids::addBoid(Boid b)
{
    b.setWanderStrength(WANDER_STRENGTH);
    listeBoids.push_back(b);
}

void ListeBoids::addBoid()
{
    listeBoids.push_back(Boid{WANDER_STRENGTH});
}

void ListeBoids::update(float separation, float cohesion, float alignment)
{
    for (Boid& b : listeBoids)
    {
        b.applySteeringForces(listeBoids, separation*FACTOR, cohesion*FACTOR, alignment*FACTOR);
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
