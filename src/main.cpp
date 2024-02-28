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
    auto ctx = p6::Context{{.title = "prog-web-s4"}};
    ctx.maximize_window();

    Boid b;
    ListeBoids listeBoids;
    for (int i=0 ; i<10 ; i++) {
        listeBoids.newBoid();
    }
    listeBoids.addBoid(b);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Jade);
        listeBoids.update();
        listeBoids.display(ctx);

        b.updatePosition();
        b.display(ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}