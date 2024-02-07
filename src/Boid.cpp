#include "Boid.hpp"
#include "p6/p6.h"
#include <iostream>
#include <time.h>

Boid::Boid()
: position(Boid::DIMENSION), direction(Boid::DIMENSION) {
    srand(time(NULL));
    color = {static_cast<float>(rand()%256)/256 , static_cast<float>(rand()%256)/256 , static_cast<float>(rand()%256)/256};
    for (int i=0 ; i<Boid::DIMENSION ; i++) {
        direction[i] = static_cast<float>(rand()%100-50)/10000;
    }
}

void Boid::display(p6::Context &ctx) {
    ctx.circle(
            p6::Center{position[0],position[1]},
            p6::Radius{0.2f}
        );
}

void Boid::updatePosition() {
    for (int i=0 ; i<Boid::DIMENSION ; i++) {
        direction[i] = static_cast<float>(rand()%100-50)/1000;
        position[i] += direction[i];
    
}
}