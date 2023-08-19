#pragma once
#include "ComponentPropertyKit.h"

class Simulation {
public:
    Simulation();

    void loop();
private:
    component_property_kit_lib::Properties mProperties;
};

