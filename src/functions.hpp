#pragma once

#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include "glimac/tiny_obj_loader.h"
#include "glimac/common.hpp"


#include "vbo.hpp"
#include "vao.hpp"

const GLuint VERTEX_ATTR_POSITION = 0;
const GLuint VERTEX_ATTR_NORMAL = 1;
const GLuint VERTEX_ATTR_TEXCOORDS = 2;

float rand01();

struct vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Object3D{
    std::vector<glimac::ShapeVertex> vertices;
};

Object3D loadOBJ(const char * path);

void enableVertex(VBO &vbo, VAO &vao,  std::vector<glimac::ShapeVertex> vertices);

void calculateMarkovState(glm::vec4 &currentState, glm::mat4 matrix);

double uniform(double a, double b);

std::vector<int> generateHyperGeometric(int nombreTotal, int essaie, int nombreSucces);

std::vector<int> generateBernoulliSchema(double probaSuccess, int essaie);

double generateExponential(double lambda);

double geometricTrial(double p);

int poissonRandom(double lambda);

double laplaceRandom(double mu, double b);

