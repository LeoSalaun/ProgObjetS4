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
    void                   display(glm::mat4 &ModelMatrix, glm::mat4 &ViewMatrix, glm::mat4 &MVMatrix, glm::mat4 &NormalMatrix, const glm::mat4 ProjMatrix,
                           const GLint uMVPMatrix, const GLint uMVMatrix, const GLint uNormalMatrix) const;
};