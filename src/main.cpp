#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "structures.cpp"
#include "Boid.hpp"
#include "ListeBoids.hpp"

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Les boids ils sont partout, dans les villes, les campagnes, sur les réseaux sociaaaauuuuux..."}};
    ctx.maximize_window();

    ListeBoids listeBoids{};
    for (int i=0 ; i<60 ; i++) {
        listeBoids.addBoid(Boid{});
    }

    auto separation = 5.f;
    auto cohesion = .05f;
    auto alignment = 50.f;


    ctx.imgui = [&]() {
        ImGui::Begin("Steering forces");
        ImGui::SliderFloat("Separation strength", &separation, 0.f, 10.f );
        ImGui::SliderFloat("Cohesion strength",   &cohesion,   0.f, .1f  );
        ImGui::SliderFloat("Alignment strength",  &alignment,  0.f, 100.f);
        ImGui::End();
        ImGui::ShowDemoWindow();
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Black);

        ctx.fill = {1, 1, 1, 1};
        ctx.square(
            p6::Center{0,0},
            p6::Radius{0.52f}
        );

        ctx.fill = {0.75, 0, 0, 1};
        ctx.stroke_weight = 0.f;
        listeBoids.update(separation, cohesion, alignment);
        listeBoids.display(ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}