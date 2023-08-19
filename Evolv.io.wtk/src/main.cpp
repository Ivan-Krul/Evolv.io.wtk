#include <iostream>

#include "raylib.h"

#include "Simulation.h"

int main() {
    InitWindow(640, 480, "Evolv.io.wtk");
    SetTargetFPS(60);

    Simulation sim;

    while (!WindowShouldClose()) {
        sim.loop();
    }

    CloseWindow();
    return 0;
}
