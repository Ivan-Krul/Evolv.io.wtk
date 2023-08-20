#pragma once
#include <stdint.h>
#include <bitset>

#include "raylib.h"

class EnviroumentWaterService {
public:
    EnviroumentWaterService();

    void setSeaLevel(uint16_t seaLevel) noexcept;
    void setTides(float tideAmplitude, float tideFrequency) noexcept;

    bool isReady() const noexcept;

    _NODISCARD uint16_t calculateSeaLevel() const noexcept;
    _NODISCARD uint16_t getHighestLevel() const noexcept;
    _NODISCARD uint16_t getLowestLevel() const noexcept;
    void takeStep(float oscillator);

private:
    std::bitset<2> mCheckList;

    uint16_t mSeaLevel; // in procents
    float mTideFrequency;
    float mTideAmplitude;
    float mTideRatio = 0.5;
};

