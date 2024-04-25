#include <cstdlib>
#include <iostream>
#include <ostream>
#include "glm/trigonometric.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "Boid.hpp"
#include "ListeBoids.hpp"
#include "Model3D.hpp"
#include "Texture.hpp"
#include "doctest/doctest.h"
#include "functions.hpp"
// #include "structures.cpp"
#include <img/src/Image.h>
#include <vector>
#include "functions.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include <img/src/Image.h>

using namespace glimac;

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Astéboïdes !!!"}};
    ctx.maximize_window();

    const p6::Shader shader = p6::load_shader(
        "src/shaders/3D.vs.glsl",
        "src/shaders/allLights.fs.glsl"
    );

    double     number   = uniform(10, 50);
    int        intValue = static_cast<int>(number);
    double     meanSize = 10.0;
    ListeBoids listeBoids{};
    for (int i = 0; i < intValue; i++)
    {
        int size = poissonRandom(meanSize);
        listeBoids.addBoid(Boid{size});
    }

    // GET UNIFORM VARIABLES

    GLint uMVPMatrix    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLint uMVMatrix     = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLint uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");
    GLint uTexture      = glGetUniformLocation(shader.id(), "uTexture");

    // Calculate random materials for the lighting

    // glm::vec3 kd{rand01(),rand01(),rand01()};
    // glm::vec3 ks{rand01(),rand01(),rand01()};

    GLint uKdCenter             = glGetUniformLocation(shader.id(), "uKdCenter");
    GLint uKsCenter             = glGetUniformLocation(shader.id(), "uKsCenter");
    GLint uShininessCenter      = glGetUniformLocation(shader.id(), "uShininessCenter");
    GLint uLightPosCenter       = glGetUniformLocation(shader.id(), "uLightPosCenter");
    GLint uLightIntensityCenter = glGetUniformLocation(shader.id(), "uLightIntensityCenter");

    GLint uKdSelf             = glGetUniformLocation(shader.id(), "uKdSelf");
    GLint uKsSelf             = glGetUniformLocation(shader.id(), "uKsSelf");
    GLint uShininessSelf      = glGetUniformLocation(shader.id(), "uShininessSelf");
    GLint uLightPosSelf       = glGetUniformLocation(shader.id(), "uLightPosSelf");
    GLint uLightIntensitySelf = glGetUniformLocation(shader.id(), "uLightIntensitySelf");

    // LOAD 3D MODELS

    Model3D sphere{glimac::sphere_vertices(1.f, 64.f, 32.f)};
    Model3D planet{loadOBJ("assets/models/planet.obj")};
    Model3D ufo{loadOBJ("assets/models/ufo.obj")};
    Model3D cube{loadOBJ("assets/models/cube.obj")};
    Model3D star{loadOBJ("assets/models/star.obj")};
    Model3D solrock{loadOBJ("assets/models/Solrock.obj")};
    Model3D lunatone{loadOBJ("assets/models/Lunatone.obj")};

    // LOAD TEXTURES

    Texture SpaceMap("assets/textures/SpaceMap.jpg");
    Texture SunMap("assets/textures/SunMap.jpg");
    Texture PlanetMap("assets/textures/PlanetMap.jpg");
    Texture MoonMap("assets/textures/MoonMap.jpg");
    Texture Gold("assets/textures/gold.jpg");
    Texture Metal("assets/textures/metal.jpg");
    Texture Rainbow("assets/textures/rainbow.jpg");

    glEnable(GL_DEPTH_TEST);

    FreeflyCamera camera;
    camera.moveFront(15.f);


    // MARKOV CHAIN


    glm::mat4 markovMatrixSun{glm::vec4{ .6f,.05f,.25f, .2f},
                              glm::vec4{ .0f, .9f,.15f,.35f},
                              glm::vec4{.25f, .0f,.55f, .0f},
                              glm::vec4{.15f,.05f, .0f,.45f}};

    glm::mat4 markovMatrixMoon{glm::vec4{ .3f, .0f,.8f,.5f},
                               glm::vec4{.16f,.15f,.1f,.0f},
                               glm::vec4{.14f,.15f,.1f,.0f},
                               glm::vec4{ .4f, .7f,.0f,.5f}};

    glm::vec4 stateSun{0.f,0.f,1.f,0.f};
    glm::vec4 stateMoon{1.f,0.f,0.f,0.f};

    glm::vec2 markovPositionSun{0.f,0.f};
    glm::vec2 markovPositionMoon{0.f,0.f};




    int frame = 0;

    int updatePlanetFrame1 = static_cast<int>(generateExponential(1.f/180.f));
    float speed1 = static_cast<float>(uniform(0.1, 1.));
    float positionPlanet1 = 0.f;

    int updatePlanetFrame2 = static_cast<int>(generateExponential(1.f/300.f));
    float speed2 = static_cast<float>(uniform(0.1, 1.));
    float positionPlanet2 = 0.f;




    // INTERFACE MANAGEMENT

    auto separation = 5.f;
    auto cohesion   = .05f;
    auto alignment  = 50.f;

    ctx.imgui = [&]() {
        ImGui::Begin("Steering forces");
        ImGui::SliderFloat("Separation strength", &separation, 0.f, 10.f);
        ImGui::SliderFloat("Cohesion strength", &cohesion, 0.f, .1f);
        ImGui::SliderFloat("Alignment strength", &alignment, 0.f, 100.f);
        ImGui::End();
    };

    // Variables aléatoires lumière diffuse
    std::vector<int> dif = generateBernoulliSchema(0.5, 3);
    // Variables aléatoires lumière gloossy
    std::vector<int> glo = generateBernoulliSchema(0.5, 3);
    // Variables aléatoires intensité lumière
    std::vector<int> kd = generateHyperGeometric(20, 3, 15);
    std::vector<int> ks = generateHyperGeometric(20, 3, 15);

    double tailleUfo = std::abs(laplaceRandom(0, 0.5));
    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glUniform1i(uTexture, 0);

        frame++;

        // CAMERA MANAGEMENT

        if (ctx.key_is_pressed(GLFW_KEY_W))
            camera.moveFront(-0.1f);
        if (ctx.key_is_pressed(GLFW_KEY_A))
            camera.moveLeft(-0.1f);
        if (ctx.key_is_pressed(GLFW_KEY_S))
            camera.moveFront(0.1f);
        if (ctx.key_is_pressed(GLFW_KEY_D))
            camera.moveLeft(0.1f);

        if (ctx.mouse_button_is_pressed(p6::Button::Right))
        {
            camera.rotateLeft(-ctx.mouse_delta().x * 100.f);
            camera.rotateUp(ctx.mouse_delta().y * 100.f);
        }

        // CREATE MATRIX
      
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 ViewMatrix = camera.getViewMatrix();
        glm::mat4 ModelMatrix;



        // DRAW OBJECTS

        // DRAW SUN AND MOON

        // SUN

        SunMap.bind();

        if (static_cast<int>(frame) % 20 == 0) {
            calculateMarkovState(stateSun, markovMatrixSun);
        }
        markovPositionSun.x += stateSun[0];
        markovPositionSun.x -= stateSun[1];
        markovPositionSun.y += stateSun[2];
        markovPositionSun.y -= stateSun[3];
        
        ModelMatrix = glm::mat4(1.f);
        ModelMatrix = glm::rotate(ModelMatrix, markovPositionSun.x/100.f, {0.f, 1.f, 0.f});
        ModelMatrix = glm::rotate(ModelMatrix, markovPositionSun.y/100.f, {0.f, 0.f, 1.f});
        ModelMatrix = glm::translate(ModelMatrix, vec(40.f,0.f,0.f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(.2f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(90.f), {0.f, 1.f, 0.f});

        solrock.drawObject(ViewMatrix, ModelMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix);
      
        // APPLY LIGHTING TO SUN
      
        glUniform3f(uKdCenter, (float)dif[0], (float)dif[1], (float)dif[2]);           
        glUniform3f(uKsCenter, (float)glo[0], (float)glo[1], (float)glo[2]);
        glUniform1f(uShininessCenter, 1.f);
        glm::vec4 lightPos = ViewMatrix*glm::translate(ModelMatrix, vec(0.f,55.f,0.f))*glm::vec4(1.f, 1.f, 1.f, 1.f);
        glUniform3f(uLightPosCenter, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(uLightIntensityCenter, (float)kd[0] * 500.f, (float)kd[1] * 500.f, (float)kd[2] * 500.f);

        SunMap.unbind();

        // MOON

        MoonMap.bind();

        if (static_cast<int>(frame) % 20 == 0) {
            calculateMarkovState(stateMoon, markovMatrixMoon);
        }
        markovPositionMoon.x += stateMoon[0];
        markovPositionMoon.x -= stateMoon[1];
        markovPositionMoon.y += stateMoon[2];
        markovPositionMoon.y -= stateMoon[3];

        ModelMatrix = glm::mat4(1.f);
        ModelMatrix = glm::rotate(ModelMatrix, markovPositionMoon.x/100.f, {0.f, 1.f, 0.f});
        ModelMatrix = glm::rotate(ModelMatrix, markovPositionMoon.y/100.f, {0.f, 0.f, 1.f});
        ModelMatrix = glm::translate(ModelMatrix, vec(40.f,0.f,0.f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(.2f));

        lunatone.drawObject(ViewMatrix, ModelMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix);

        glUniform3f(uKdSelf, (float)dif[0], (float)dif[1], (float)dif[2]);
        glUniform3f(uKsSelf, (float)glo[0], (float)glo[1], (float)glo[2]);
        glUniform1f(uShininessSelf, 1.f);
        lightPos = glm::vec4(1.f, 1.f, 1.f, 1.f);
        glUniform3f(uLightPosSelf, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(uLightIntensitySelf, (float)ks[0] * 25.f, (float)ks[1] * 25.f, (float)ks[2] * 25.f);

        MoonMap.unbind();

        // DRAW CUBE

        Gold.bind();

        ModelMatrix = glm::mat4(1.f);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.23f));

        cube.drawObject(ViewMatrix, ModelMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix);

        Gold.unbind();

        // DRAW ARPENTEUR

        Metal.bind();

        ModelMatrix = glm::inverse(ViewMatrix);
        ModelMatrix = glm::translate(ModelMatrix, vec(0.f, -2.f, -5.f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(180.f), {1.f, 0.f, 0.f});
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(tailleUfo + 0.5f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(90.f), {0.f, 1.f, 0.f});

        ufo.drawObject(ViewMatrix, ModelMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix);

        Metal.unbind();

        // DRAW PLANETS THAT ROTATE AROUND THE BOIDS

        PlanetMap.bind();

        if (frame == updatePlanetFrame1) {
            updatePlanetFrame1 += static_cast<float>(generateExponential(1.f/180.f));
            speed1 = static_cast<float>(uniform(0.1, 1.));
        }

        positionPlanet1 += speed1;

        ModelMatrix = glm::mat4(1.f);
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(180.f), {0.f, 0.f, 11.f});
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(positionPlanet1), {0.f, 1.f, 0.f});
        ModelMatrix = glm::translate(ModelMatrix, vec(30.f,0.f,0.f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5.f));

        planet.drawObject(ViewMatrix, ModelMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix);

        if (frame == updatePlanetFrame2) {
            updatePlanetFrame2 += static_cast<float>(generateExponential(1.f/300.f));
            speed2 = static_cast<float>(uniform(0.1, 1.));
        }

        positionPlanet2 += speed2;

        ModelMatrix = glm::mat4(1.f);
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(300.f), {0.f, 0.f, 11.f});
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(positionPlanet2), {0.f, 1.f, 0.f});
        ModelMatrix = glm::translate(ModelMatrix, vec(35.f,0.f,0.f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5.f));

        planet.drawObject(ViewMatrix, ModelMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix);

        PlanetMap.unbind();

        // DRAW BACKGROUND

        SpaceMap.bind();

        ModelMatrix = glm::mat4(1.f);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(50.f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(frame/40.f), {0.f, 1.f, 0.5f});

        sphere.drawObject(ViewMatrix, ModelMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix);

        SpaceMap.unbind();

        // DRAW BOIDS

        Rainbow.bind();

        listeBoids.update(separation, cohesion, alignment);
        listeBoids.display(ModelMatrix, ViewMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix, star);

        Rainbow.unbind();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}