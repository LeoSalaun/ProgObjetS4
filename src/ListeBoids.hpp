#pragma once
#include <vector>
#include "Boid.hpp"

class ListeBoids {
private:
    std::vector<Boid> listeBoids;
    vec               averagePosition;

public:
    ListeBoids();

    void newBoid();
    void addBoid(Boid b);
    void update();
    void display(p6::Context& ctx) const;
    vec  calculateCohesionForce();
};