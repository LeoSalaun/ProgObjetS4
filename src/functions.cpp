#include "functions.hpp"
#include <random>
#include <iostream>
#include "glimac/common.hpp"
#include "glm/common.hpp"
#include "glm/ext/quaternion_geometric.hpp"

float rand01()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<float>{0.0, 1.0};

    return distrib(gen);
}

Object3D loadOBJ(const char * path) { // LOAD OBJECT FROM PATH
    Object3D toreturn;

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::vector<float> positions;

    std::cout << tinyobj::LoadObj(shapes, materials, path);

    for(auto s : shapes[0].mesh.positions) positions.push_back(s);
    for(auto i : shapes[0].mesh.indices) {
        glimac::ShapeVertex tmp;
        tmp.position = glm::vec3(positions[i*3], positions[i*3 + 1], positions[i*3+2]);
        tmp.normal = glm::vec3(shapes[0].mesh.normals[i*3], shapes[0].mesh.normals[i*3+1], shapes[0].mesh.normals[i*3+2]);
        tmp.texCoords = glm::vec2(shapes[0].mesh.texcoords[i*2], shapes[0].mesh.texcoords[i*2+1]);
        toreturn.vertices.push_back(tmp);
    } 
    return toreturn;
}

void enableVertex(VBO &vbo, VAO &vao,  std::vector<glimac::ShapeVertex> vertices) { // ENABLE VBO AND VAO
    vbo.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, static_cast<glm::int64>(vertices.size() * sizeof(glimac::ShapeVertex)), vertices.data(), GL_STATIC_DRAW);
    vbo.unbind(GL_ARRAY_BUFFER);

    vao.bind();
    vbo.bind(GL_ARRAY_BUFFER);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
    
    vbo.unbind(GL_ARRAY_BUFFER);
    vao.unbind();
}

void calculateMarkovState(glm::vec4 &currentState, glm::mat4 matrix) {
    glm::vec4 newProba = matrix*currentState;

    float randomValue = rand01();
    float randomSum = 0.f;
    int state = 0;

    for (state = 0; state < 4; ++state) {
        randomSum += newProba[state];
        if (randomValue <= randomSum) {
            break;
        }
    }

    
    // std::cout << state << " " << randomSum << std::endl;
    // std::cout << newProba[0] << " " << newProba[1]  << " " << newProba[2]  << " " << newProba[3] << std::endl;

    switch (state) {
        case 0  : currentState = {1.f, 0.f, 0.f, 0.f};
                  break;
        case 1  : currentState = {0.f, 1.f, 0.f, 0.f};
                  break;
        case 2  : currentState = {0.f, 0.f, 1.f, 0.f};
                  break;
        case 3  : currentState = {0.f, 0.f, 0.f, 1.f};
                  break;
        default : break;
    }
}