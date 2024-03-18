#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "p6/p6.h"
#include "structures.cpp"
#include "Boid.hpp"
#include "ListeBoids.hpp"
#include <vector>

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Les boids ils sont partout, dans les villes, les campagnes, sur les réseaux sociaaaauuuuux..."}};
    ctx.maximize_window();

    Boid b{};
    ListeBoids listeBoids{};
    for (int i=0 ; i<10 ; i++) {
        listeBoids.addBoid(Boid{});
    }
   // listeBoids.addBoid(b);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Red);

        ctx.fill = {1, 1, 1, 1};
        ctx.square(
            p6::Center{0,0},
            p6::Radius{0.52f}
        );

        ctx.fill = {0.75, 0, 0, 1};
        ctx.stroke_weight = 0.f;
        listeBoids.update();
        listeBoids.display(ctx);

        //b.updatePosition();
        b.display(ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}