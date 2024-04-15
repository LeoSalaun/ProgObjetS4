#include "functions.hpp"
#include <random>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

float rand01()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<float>{0.0, 1.0};

    return distrib(gen);
}