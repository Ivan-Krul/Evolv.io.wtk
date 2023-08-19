#include <iostream>

#include "raylib.h"

int main() {
    InitWindow(640, 480, "Evolv.io.wtk");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("this text should show for me!", 100, 100, 24, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
