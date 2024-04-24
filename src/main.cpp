#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
// #include "structures.cpp"
#include <img/src/Image.h>
#include <vector>
#include "Boid.hpp"
#include "ListeBoids.hpp"
#include "functions.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
#include "vao.hpp"
#include "vbo.hpp"

using namespace glimac;

const GLuint VERTEX_ATTR_POSITION  = 0;
const GLuint VERTEX_ATTR_NORMAL    = 1;
const GLuint VERTEX_ATTR_TEXCOORDS = 2;



int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Les boids ils sont partout, dans les villes, les campagnes, sur les réseaux sociaaaauuuuux..."}};
    ctx.maximize_window();

    const p6::Shader shader = p6::load_shader(
        "src/shaders/3D.vs.glsl",
        "src/shaders/directionalLight.fs.glsl"
    );

    ListeBoids listeBoids{};
    for (int i = 0; i < 60; i++)
    {
        listeBoids.addBoid(Boid{});
    }

    GLint uMVPMatrix    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLint uMVMatrix     = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLint uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");
    GLint uTexture      = glGetUniformLocation(shader.id(), "uTexture");
    GLint uColor        = glGetUniformLocation(shader.id(), "uColor");

    GLint uKd             = glGetUniformLocation(shader.id(), "uKd");
    GLint uKs             = glGetUniformLocation(shader.id(), "uKs");
    GLint uShininess      = glGetUniformLocation(shader.id(), "uShininess");
    GLint uLightDir_vs    = glGetUniformLocation(shader.id(), "uLightDir_vs");
    GLint uLightIntensity = glGetUniformLocation(shader.id(), "uLightIntensity");

    glEnable(GL_DEPTH_TEST);

    VBO vbo;
    VAO vao;

    vbo.bind(GL_ARRAY_BUFFER);

    std::vector<ShapeVertex> vertices = glimac::sphere_vertices(1.f, 64.f, 32.f);

    glBufferData(GL_ARRAY_BUFFER, static_cast<glm::int64>(vertices.size() * sizeof(ShapeVertex)), vertices.data(), GL_STATIC_DRAW);

    vao.bind();

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, texCoords));

    vbo.unbind(GL_ARRAY_BUFFER);
    vao.unbind();

    FreeflyCamera camera;
    camera.moveFront(15.f);

    glm::vec3 kd{rand01(), rand01(), rand01()};
    glm::vec3 ks{rand01(), rand01(), rand01()};

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

    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vao.bind();

        shader.use();

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

        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 ViewMatrix   = camera.getViewMatrix();
        glm::mat4 ModelMatrix  = glm::mat4(1.f);
        glm::mat4 MVMatrix     = ViewMatrix * ModelMatrix;
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniform3f(uKd, kd.x, kd.y, kd.z);
        glUniform3f(uKs, ks.x, ks.y, ks.z);
        glUniform1f(uShininess, 1.f);
        glm::vec4 lightDir = ViewMatrix * glm::vec4(1.f, 1.f, 1.f, 0.f);
        glUniform3f(uLightDir_vs, lightDir.x, lightDir.y, lightDir.z);
        glUniform3f(uLightIntensity, 1.f, 1.f, 1.f);

        MVMatrix     = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        glUniformMatrix4fv(uMVPMatrix, 1.f, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix, 1.f, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1.f, GL_FALSE, glm::value_ptr(NormalMatrix));



    


            glUniform3f(uColor, 1.f, 1.f, 1.f);

        // ctx.fill = {1, 1, 1, 1};
        // ctx.square(
        //     p6::Center{0,0},
        //     p6::Radius{0.52f}
        // );

        // ctx.fill = {0.75, 0, 0, 1};
        // ctx.stroke_weight = 0.f;
        listeBoids.update(separation, cohesion, alignment);
        listeBoids.display(ModelMatrix, ViewMatrix, MVMatrix, NormalMatrix, ProjMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix);

        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(vertices.size()));

        vao.unbind();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}