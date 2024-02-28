#include "ListeBoids.hpp"
#include "Boid.hpp"
#include <iostream>
#include <ostream>
#include <vector>

const float ALIGNMENT_STRENGTH = 1/1000;
const float COHESION_STRENGTH = 1/10;

ListeBoids::ListeBoids()
: listeBoids() {}

void ListeBoids::newBoid() {
    Boid b;
    listeBoids.push_back(b);
}

void ListeBoids::addBoid(Boid b) {
    listeBoids.push_back(b);
}

void ListeBoids::update() {
    for (Boid &b : listeBoids)
    {
        b.CalculateSeparationForce(listeBoids);
        vec cohesionForce = calculateCohesionForce();
        b.updatePosition(cohesionForce);
    }
}

void ListeBoids::display(p6::Context &ctx) const {
    for (Boid b : listeBoids)
    {
        b.display(ctx);
    }
}

vec ListeBoids::calculateCohesionForce() {
    averagePosition = vec(0,0);
    for (const Boid &b : listeBoids)
    {
        averagePosition += b.getPosition();
    }
    averagePosition /= listeBoids.size();
    return averagePosition*COHESION_STRENGTH;
}

