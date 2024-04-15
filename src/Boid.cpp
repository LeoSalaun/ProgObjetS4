#include "Boid.hpp"
#include "functions.hpp"
#include <vector>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"


#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glimac/sphere_vertices.hpp"

Boid::Boid()
 : position{vec((rand01() - 0.5f) * 0.8f, (rand01() - 0.5f) * 0.8f, (rand01() - 0.5f) * 0.8f)},
   color{ glm::vec3(rand01(), rand01(), rand01())},
   direction{vec((rand01() - 0.5f) * 0.001f, (rand01() - 0.5f) * 0.001f, (rand01() - 0.5f) * 0.001f)},
   vertices{glimac::sphere_vertices(1.f, 64.f, 32.f)} {}

Boid::Boid(float wander)
 : position{vec((rand01() - 0.5f) * 0.8f, (rand01() - 0.5f) * 0.8f, (rand01() - 0.5f) * 0.8f)},
   color{ glm::vec3(rand01(), rand01(), rand01())},
   direction{vec((rand01() - 0.5f) * wander, (rand01() - 0.5f) * wander, (rand01() - 0.5f) * wander)},
   vertices{glimac::sphere_vertices(1.f, 64.f, 32.f)}  {}

void Boid::display(glm::mat4 &ModelMatrix, glm::mat4 &ViewMatrix, glm::mat4 &MVMatrix, glm::mat4 &NormalMatrix, const glm::mat4 ProjMatrix,
                   const GLint uMVPMatrix, const GLint uMVMatrix, const GLint uNormalMatrix) const
{
    // ctx.circle(
    //     p6::Center{position[0], position[1]},
    //     p6::Radius{0.02f}
    // );




    ModelMatrix = glm::mat4(1.f);

    ModelMatrix = glm::translate(ModelMatrix, {20.f*position.x, 20.f*position.y, 20.f*position.z});
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.2f));

    MVMatrix = ViewMatrix*ModelMatrix;
    NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    glUniformMatrix4fv(uMVPMatrix, 1.f, GL_FALSE, glm::value_ptr(ProjMatrix*MVMatrix));
    glUniformMatrix4fv(uMVMatrix, 1.f, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrix, 1.f, GL_FALSE, glm::value_ptr(NormalMatrix));

    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(vertices.size()));
}

void Boid::updatePosition()
{
    position += direction;

    if (position.x >  0.5f)
        position.x -= 1.f;
    if (position.x < -0.5f)
        position.x += 1.f;

    if (position.y >  0.5f)
        position.y -= 1.f;
    if (position.y < -0.5f)
        position.y += 1.f;
    
    if (position.z >  0.5f)
        position.z -= 1.f;
    if (position.z < -0.5f)
        position.z += 1.f;
}

vec Boid::getPosition() const
{
    return position;
}

glm::vec3 Boid::getColor() const
{
    return color;
}

vec Boid::getDirection() const
{
    return direction;
}

vec Boid::calculateSeparationForce(const std::vector<Boid>& listeBoids, float separation)
{
    vec totalForce = vec(0.f);

    for (const Boid& otherBoid : listeBoids)
    {
        if (&otherBoid == this)
            continue;
        float distance            = glm::distance(position, otherBoid.getPosition());
        vec   separationDirection = position - otherBoid.getPosition();
        if (distance > 0.f && distance < .05f) {
            totalForce += separationDirection / (distance);
        }
    }
    return totalForce * separation;
}

vec Boid::calculateCohesionForce(const std::vector<Boid>& boids, float cohesion)
{
    vec averagePosition = vec(0.f);
    for (const Boid& otherBoid : boids)
    {
        float distance = glm::distance(position, otherBoid.getPosition());
        if (distance > 0 && distance < .002f)
            averagePosition += otherBoid.getPosition();
    }
    averagePosition /= boids.size();
    return (averagePosition-position) * cohesion;
}

vec Boid::calculateAlignmentForce(const std::vector<Boid>& boids, float alignment)
{
    vec averageDirection(0.0f);

    for (const Boid& otherBoid : boids)
    {
        if (&otherBoid != this)
        {
            averageDirection += otherBoid.getDirection();
        }  
    }
     if (!boids.empty())
    {
        averageDirection /= static_cast<float>(boids.size());
    }
    if (glm::length(averageDirection) > 0) {
        return averageDirection * alignment;
    }
    return vec{0.f};
}

void Boid::applySteeringForces(const std::vector<Boid>& boids, float separation, float cohesion, float alignment) {
    vec totalForce = calculateSeparationForce(boids, separation)
                   + calculateCohesionForce(boids, cohesion)
                   + calculateAlignmentForce(boids, alignment);
    direction += glm::normalize(totalForce);
    direction *= 0.001f;
};
