#include "ListeBoids.hpp"
#include <vector>
#include "Boid.hpp"

void ListeBoids::addBoid(Boid b)
{
    listeBoids.push_back(b);
}

void ListeBoids::update(float separation, float cohesion, float alignment)
{
    for (Boid& b : listeBoids)
    {
        b.applySteeringForces(listeBoids, separation, cohesion, alignment);
        b.updatePosition();
    }
}

void ListeBoids::display(glm::mat4 &ModelMatrix, glm::mat4 &ViewMatrix, glm::mat4 &MVMatrix, glm::mat4 &NormalMatrix, const glm::mat4 ProjMatrix,
                         const GLint uMVPMatrix, const GLint uMVMatrix, const GLint uNormalMatrix) const
{
    for (const Boid& b : listeBoids)
    {
        b.display(ModelMatrix, ViewMatrix, MVMatrix, NormalMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix);
    }
}
