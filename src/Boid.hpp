#pragma once
#include <string>
#include <vector>
#include "p6/p6.h"

class Boid {
private:
    std::vector<float> position;
    std::vector<float> color;
    std::vector<float> speed;

public:
    Boid();

    void display(p6::Context &ctx);
    void updatePosition();
    const static int DIMENSION = 2;
};