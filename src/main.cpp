#include <cstdlib>
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

    // LOAD TEXTURES

    Texture SpaceMap("assets/textures/SpaceMap.jpg");
    Texture MoonMap("assets/textures/MoonMap.jpg");
    Texture Gold("assets/textures/gold.jpg");
    Texture Metal("assets/textures/metal.jpg");
    Texture Rainbow("assets/textures/rainbow.jpg");

    glEnable(GL_DEPTH_TEST);

    FreeflyCamera camera;
    camera.moveFront(15.f);

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
        ImGui::ShowDemoWindow();
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

        // APPLY LIGHTING

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 ViewMatrix = camera.getViewMatrix();
        glm::mat4 ModelMatrix;

        glUniform3f(uKdCenter, (float)dif[0], (float)dif[1], (float)dif[2]);
        glUniform3f(uKsCenter, (float)glo[0], (float)glo[1], (float)glo[2]);
        glUniform1f(uShininessCenter, 1.f);
        glm::vec4 lightPos = ViewMatrix * glm::vec4(1.f, 1.f, 1.f, 1.f);
        glUniform3f(uLightPosCenter, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(uLightIntensityCenter, (float)kd[0] * 15.f, (float)kd[1] * 15.f, (float)kd[2] * 15.f);

        glUniform3f(uKdSelf, (float)dif[0], (float)dif[1], (float)dif[2]);
        glUniform3f(uKsSelf, (float)glo[0], (float)glo[1], (float)glo[2]);
        glUniform1f(uShininessSelf, 1.f);
        lightPos = glm::vec4(1.f, 1.f, 1.f, 1.f);
        glUniform3f(uLightPosSelf, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(uLightIntensitySelf, (float)ks[0] * 25.f, (float)ks[1] * 25.f, (float)ks[2] * 25.f);

        // DRAW OBJECTS

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
        std::cout << tailleUfo << std::endl;
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(tailleUfo + 0.5f));

        ufo.drawObject(ViewMatrix, ModelMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix);

        Metal.unbind();

        // DRAW PLANETS THAT ROTATE AROUND THE BOIDS

        MoonMap.bind();

        ModelMatrix = glm::mat4(1.f);
        ModelMatrix = glm::rotate(ModelMatrix, ctx.time() / 5.f, {0.f, 1.f, 0.f});
        ModelMatrix = glm::translate(ModelMatrix, vec(40.f, 0.f, 0.f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5.f));

        planet.drawObject(ViewMatrix, ModelMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix);

        ModelMatrix = glm::mat4(1.f);
        ModelMatrix = glm::rotate(ModelMatrix, 90.f, {1.f, 0.f, 0.5f});
        ModelMatrix = glm::rotate(ModelMatrix, ctx.time() / 5.f, {0.f, 1.f, 0.f});
        ModelMatrix = glm::translate(ModelMatrix, vec(30.f, 0.f, 0.f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5.f));

        planet.drawObject(ViewMatrix, ModelMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix);

        MoonMap.unbind();

        // DRAW BACKGROUND

        SpaceMap.bind();

        ModelMatrix = glm::mat4(1.f);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(50.f));
        ModelMatrix = glm::rotate(ModelMatrix, ctx.time() / 20.f, {0.f, 1.f, 0.5f});

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