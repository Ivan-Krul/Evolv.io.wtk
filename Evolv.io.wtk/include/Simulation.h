#pragma once
#include "ComponentPropertyKit.h"
#include "GraphicSystem.h"

#include "Enviroument.h"

class Simulation {
public:
    Simulation();

    void loop();
private:
    const int mWindowX = 640;
    const int mWindowY = 480;
    Enviroument mEnviroument;
    graphic_sys_lib::Renderer mRenderer;
    float mSpeed;
};

