#pragma once
#include <vector>
#include "Boid.hpp"

class ListeBoids {
private:
    std::vector<Boid>      listeBoids{};
    vec                    averagePosition{};

public:
    void                   addBoid(Boid b);
    void                   update(float separation, float cohesion, float alignment);
    void                   display(p6::Context& ctx) const;
};