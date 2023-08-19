#include "Simulation.h"

#include "raylib.h"

Simulation::Simulation() {
}

void Simulation::loop() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("This text means, what it's works!", 100, 100, 20, WHITE);

    EndDrawing();
}
