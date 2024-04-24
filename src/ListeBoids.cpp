#include "ListeBoids.hpp"
#include <vector>
#include "Boid.hpp"
#include "Model3D.hpp"

void ListeBoids::addBoid(Boid const &b)
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

void ListeBoids::display(glm::mat4 &ModelMatrix, glm::mat4 &ViewMatrix, glm::mat4 &ProjMatrix,
                         const GLint uMVPMatrix, const GLint uMVMatrix, const GLint uNormalMatrix, Model3D &model) const
{
    for (const Boid& b : listeBoids)
    {
        b.display(ModelMatrix, ViewMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix, model);
    }
}
