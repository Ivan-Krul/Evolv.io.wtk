#include "Simulation.h"

#include "raylib.h"

Simulation::Simulation()
    : mEnviroument(640, 480) {

    mEnviroument.getWaterService().setSeaLevel(64);
    mEnviroument.getWaterService().setTides(5, 30);
    mEnviroument.setTempratureRange(15,20,30);
    mEnviroument.setSeasonFrequency(0.1);

    mRenderer.push<graphic_sys_lib::Sprite>(LoadTextureFromImage(mEnviroument.getImage()));

    mSpeed = 0.01;
}

void Simulation::loop() {
    if (IsKeyDown(KEY_UP)) {
        mSpeed += 0.001;
    }
    if (IsKeyDown(KEY_DOWN)) {
        mSpeed -= 0.001;
    }

    mEnviroument.step(mSpeed);

    if (std::fmod(mEnviroument.getOscillator(), 0.5) < mSpeed) {
        mEnviroument.generateImage();
        UpdateTexture(mRenderer.element<graphic_sys_lib::Sprite>(0).getTexuture(), mEnviroument.getImage().data);
        
    }

    BeginDrawing();
    ClearBackground(BLACK);

    mRenderer.render();

    DrawRectangle(0, 0, 175, 20, BLACK);
    DrawText(std::to_string(mSpeed).c_str(), 85, 0, 20, GRAY);
    
    DrawFPS(0, 0);
    EndDrawing();
}
