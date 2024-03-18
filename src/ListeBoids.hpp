#pragma once
#include <vector>
#include "Boid.hpp"

class ListeBoids {
private:
    std::vector<Boid> listeBoids{};
    vec               averagePosition{};

    static constexpr float WANDER_STRENGTH     {1.f / 100000.f};
    static constexpr float FACTOR              {1.f / 1000000.f};

public:
    void addBoid(Boid b);
    void addBoid();
    void update(float separation, float cohesion, float alignment);
    void display(p6::Context& ctx) const;
};