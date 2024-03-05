#pragma once
#include "Boid.hpp"
#include <vector>

class ListeBoids {
    private:
        std::vector<Boid> listeBoids;
        vec averagePosition;
    
    public:
        ListeBoids();

        void newBoid();
        void addBoid(Boid b);
        void update();
        void display(p6::Context &ctx) const;
        vec calculateCohesionForce();
};