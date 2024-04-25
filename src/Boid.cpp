#include "Boid.hpp"

#include "Model3D.hpp"
#include "functions.hpp"
#include <cstdlib> // Pour rand()
#include <ctime>   // Pour time()
#include <vector>
#include "functions.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


Boid::Boid()
    : position{vec((rand01() - 0.5f) * 0.8f, (rand01() - 0.5f) * 0.8f, (rand01() - 0.5f) * 0.8f)},
    //    color{ glm::vec3(rand01(), rand01(), rand01())},
    direction{vec((rand01() - 0.5f) * 0.001f, (rand01() - 0.5f) * 0.001f, (rand01() - 0.5f) * 0.001f)}
    //, vertices{glimac::sphere_vertices(1.f, 64.f, 32.f)}
    {
        // Nuances aléatoires  avec BERNOULLI
        float probability = 0.5f; // Probabilité d'accentuer la composante rouge
        srand(time(nullptr));

        // Comparaison valeurs à la probabilité p de Bernoulli


        float r = (rand() / (RAND_MAX + 1.0f)) < probability ? (rand() / (RAND_MAX + 1.0f)) : (rand() / (RAND_MAX + 1.0f)) * 0.5f; // Rouge
        float g = (rand() / (RAND_MAX + 1.0f)) < probability ? (rand() / (RAND_MAX + 1.0f)) : (rand() / (RAND_MAX + 1.0f)) * 0.5f; // Vert
        float b = (rand() / (RAND_MAX + 1.0f)) < probability ? (rand() / (RAND_MAX + 1.0f)) : (rand() / (RAND_MAX + 1.0f)) * 0.5f; // Bleu
    
        // Normaliser les valeurs de couleur pour qu'elles soient dans l'intervalle [0, 1]
        r = std::clamp(r, 0.0f, 1.0f);
        g = std::clamp(g, 0.0f, 1.0f);
        b = std::clamp(b, 0.0f, 1.0f);
    
        // std::cout<<color[0];
        // std::cout<<color[1];
        // std::cout<<color[2];
}
void Boid::display(glm::mat4 &ModelMatrix, glm::mat4 &ViewMatrix, glm::mat4 &ProjMatrix,
                   const GLint uMVPMatrix, const GLint uMVMatrix, const GLint uNormalMatrix, Model3D &model) const
{
    // glm::vec3 normalizedDirection = glm::normalize(direction);
    // glm::vec3 up = glm::vec3{0.f, 1.f, 0.f};
    // glm::vec3 axis = glm::normalize(glm::cross(normalizedDirection, up));
    // float angle = glm::degrees(acos(glm::dot(up, normalizedDirection)));         EFFECTIVE CALCULATIONS FOR A ROTATION MATRIX, HOWEVER IS DOES NOT LOOK GOOD WITH OUR BOIDS


    ModelMatrix = glm::mat4(1.f);
    ModelMatrix = glm::translate(ModelMatrix, {20.f*position.x, 20.f*position.y, 20.f*position.z}); // CALCULATE TRANSLATION MATRIX
    //ModelMatrix = glm::rotate(ModelMatrix, angle, axis);
    model.drawObject(ViewMatrix, ModelMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix); // DRAW
}

void Boid::updatePosition()
{
    position += direction;

    if (position.x >  0.5f) // REPLACE THE BOID SO THAT IT REMAINS IN THE CUBE
        position.x -= 1.f;
    if (position.x < -0.5f)
        position.x += 1.f;

    if (position.y > 0.5f)
        position.y -= 1.f;
    if (position.y < -0.5f)
        position.y += 1.f;

    if (position.z > 0.5f)
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
        if (distance > 0.f && distance < .05f)
        {
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
    return (averagePosition - position) * cohesion;
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
    if (glm::length(averageDirection) > 0)
    {
        return averageDirection * alignment;
    }
    return vec{0.f};
}

void Boid::applySteeringForces(const std::vector<Boid>& boids, float separation, float cohesion, float alignment)
{
    vec totalForce = calculateSeparationForce(boids, separation)
                     + calculateCohesionForce(boids, cohesion)
                     + calculateAlignmentForce(boids, alignment);
    direction += glm::normalize(totalForce);
    direction *= 0.001f;
};
