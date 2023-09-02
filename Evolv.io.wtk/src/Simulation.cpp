#include "Simulation.h"

#include "raylib.h"

Simulation::Simulation()
    : mEnviroument(mWindowX, mWindowY) {

    int iters =5;
    mEnviroument.getTerrainService().generate(mWindowX, mWindowY, iters);
    mEnviroument.getTerrainService().calculateSteepnessLevel(1 << (iters - 1));
    mEnviroument.getWaterService().setSeaLevel(127);
    mEnviroument.getWaterService().setTides(5, 30);
    mEnviroument.getTemperatureService().setTempratureRange(15,20,30);
    mEnviroument.getTemperatureService().setSeasonFrequency(1.1);
    mEnviroument.getCloudService().setCloudHeight(200);
    mEnviroument.getCloudService().setPlain(mWindowX, mWindowY);
    mEnviroument.getCloudService().setTopologyOfRegions(mEnviroument.getTerrainService(), mEnviroument.getWaterService());
    mEnviroument.getCloudService().setWind(-2, -2);
    mEnviroument.getCloudService().setTriggerRain(192);
    mEnviroument.getCloudService().setTriggerChainReaction(64);

    mRenderer.push<graphic_sys_lib::Sprite>(LoadTextureFromImage(mEnviroument.getImage()));

    mSpeed = 0.1;
}

void Simulation::loop() {
    //if(IsKeyPressed(KEY_SPACE))
        mEnviroument.step(mSpeed);
    mEnviroument.generateImage();
    UpdateTexture(mRenderer.element<graphic_sys_lib::Sprite>(0).getTexuture(), mEnviroument.getImage().data);

    BeginDrawing();
    ClearBackground(BLACK);

    mRenderer.render();

    DrawRectangle(0, 0, 175, 20, BLACK);
    DrawText(std::to_string(mSpeed).c_str(), 85, 0, 20, GRAY);
    
    DrawFPS(0, 0);
    EndDrawing();
}
