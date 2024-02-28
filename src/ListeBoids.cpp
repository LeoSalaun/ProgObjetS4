#include "ListeBoids.hpp"
#include "Boid.hpp"
#include <vector>

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
    for (Boid& b : listeBoids)
    {
        b.updatePosition();
    }
}

void ListeBoids::display(p6::Context &ctx) const {
    for (Boid b : listeBoids)
    {
        b.display(ctx);
    }
}

